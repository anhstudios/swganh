// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "weather_service.h"

#include <algorithm>
#include <ctime>
#include "swganh/logger.h"
#include "swganh/event_dispatcher.h"
#include "swganh/plugin/plugin_manager.h"
#include "swganh/service/service_manager.h"
#include "swganh/service/service_directory_interface.h"
#include "swganh_core/messages/server_weather_message.h"
#include "swganh/app/swganh_kernel.h"
#include "swganh_core/messages/out_of_band.h"
#include "swganh_core/simulation/simulation_service_interface.h"
#include "swganh_core/object/player/player.h"
#include "swganh_core/object/object_controller.h"
#include "swganh_core/connection/connection_client_interface.h"
#include "swganh_core/object/object.h"

using namespace std;
using namespace swganh;
using namespace swganh::service;
using namespace swganh::plugin;
using namespace swganh::messages;
using namespace swganh::simulation;
using namespace swganh::weather;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::weather;
using namespace swganh::galaxy;
using namespace swganh::scripting;

using swganh::app::KernelInterface;
using swganh::service::ServiceDescription;
using swganh::app::SwganhKernel;
using swganh::simulation::SimulationServiceInterface;


WeatherService::WeatherService(SwganhKernel* kernel)
	: kernel_(kernel), script_(kernel_->GetAppConfig().script_directory + "/weather/weather.py")
{
	player_selected_callback_ = kernel_->GetEventDispatcher()->Subscribe(
		"ObjectReadyEvent",
		[this] (shared_ptr<swganh::EventInterface> incoming_event)
		{
			const auto& player_obj = static_pointer_cast<ValueEvent<shared_ptr<Object>>>(incoming_event)->Get();
			OnPlayerEnter(player_obj);
		});
}

WeatherService::~WeatherService()
{
	weather_timer_->cancel();
}

void WeatherService::OnPlayerEnter(shared_ptr<Object> player_obj)
{
	//On object ready event, send player current scene weather message
	uint32_t playerScene = player_obj->GetSceneId();
	if(sceneLookup_.count(playerScene))
	{
		SendServerWeatherMessagePlayer_(sceneLookup_[playerScene].weatherSequence_[sceneLookup_[playerScene].sequenceCount_], player_obj->GetController());
	}
	else
	{
	}
}

ServiceDescription WeatherService::GetServiceDescription()
{
    ServiceDescription service_description(
        "WeatherService",
        "weather",
        "0.1",
        "127.0.0.1",
        0,
        0,
        0);

    return service_description;
}

Weather WeatherService::GetSceneWeather(uint32_t scene_id)
{
	if(sceneLookup_.count(scene_id))
	{
		return sceneLookup_[scene_id].weatherSequence_[sceneLookup_[scene_id].sequenceCount_].GetWeatherType();
	}
	else
		return Weather::NOSTORM;
}
void WeatherService::SetSceneWeather(uint32_t scene_id, std::vector<WeatherEvent> weather_sequence_)
{
	boost::lock_guard<boost::mutex> lock(weather_mutex_);
	if(!sceneLookup_.count(scene_id))
	{
		sceneLookup_[scene_id].scene_=Scene(scene_id);
		sceneLookup_[scene_id].weatherSequence_=weather_sequence_;
		sceneLookup_[scene_id].updateGalaxyTickTime_=0;
		sceneLookup_[scene_id].sequenceCount_=0;
	}
}

void WeatherService::SendServerWeatherMessagePlayer_(
	WeatherEvent weatherdata_,
	std::shared_ptr<swganh::observer::ObserverInterface> observer)
{
	auto controller = static_pointer_cast<swganh::object::ObjectController>(observer);
	if (controller)
	{
		ServerWeatherMessage server_weather_message;
		server_weather_message.weather_id = weatherdata_.GetWeatherType();
		server_weather_message.cloud_vector = weatherdata_.GetCloudVector();
		controller->GetRemoteClient()->SendTo(server_weather_message);			
	}
}

void WeatherService::SendServerWeatherMessageAll_(
	Weather weather_type,
	glm::vec3 cloud_vector,
    uint32_t scene_id)
{
	ServerWeatherMessage server_weather_message;
	server_weather_message.weather_id = weather_type;
	server_weather_message.cloud_vector = cloud_vector;

	simulation_service_ = kernel_->GetServiceManager()->GetService<SimulationServiceInterface>("SimulationService");
	simulation_service_->SendToScene(&server_weather_message, scene_id);
}

void WeatherService::tickPlanetWeather_()
{
	bool removeWeather = false;
	uint32_t removeWeatherScene;
	uint64_t galaxyTime = galaxy_service_->GetGalaxyTimeInMilliseconds()/1000;

	//Loop through each weather scene and check if its time to run weather sequence
	std::for_each(sceneLookup_.begin(), sceneLookup_.end(), [this, &galaxyTime, &removeWeather, &removeWeatherScene] (std::pair<const uint32_t, PlanetWeather>& planetWeather_) {
		if(planetWeather_.second.updateGalaxyTickTime_ <= galaxyTime)
		{
			WeatherEvent planetCurrentSeqeuence = planetWeather_.second.weatherSequence_.at(planetWeather_.second.sequenceCount_);
			
			if(planetWeather_.second.weatherSequence_.size()>planetWeather_.second.sequenceCount_)
			{
				SendServerWeatherMessageAll_(planetCurrentSeqeuence.GetWeatherType(),planetCurrentSeqeuence.GetCloudVector(),planetWeather_.second.scene_);
				planetWeather_.second.updateGalaxyTickTime_ = galaxyTime + (planetCurrentSeqeuence.GetDuration()*60);
				planetWeather_.second.sequenceCount_++;
			}
			//If weather sequence has finished, remove scene map
			if(planetWeather_.second.weatherSequence_.size()==planetWeather_.second.sequenceCount_)
			{
				removeWeather =true;
				removeWeatherScene=planetWeather_.second.scene_;
			}
		}
	});

	if(removeWeather)
	{
		sceneLookup_.erase(removeWeatherScene);
	}

	//Execute weather script every 30 mins
	if(weatherScriptTimer< galaxyTime)
	{
		WeatherScript();
		weatherScriptTimer = galaxyTime + 1800;
	}
}

void WeatherService::RunWeatherSequence()
{		
	weather_timer_ = std::make_shared<boost::asio::deadline_timer>(kernel_->GetCpuThreadPool(), boost::posix_time::seconds(10));
	weather_timer_->async_wait(boost::bind(&WeatherService::RunWeatherSequenceTimer, this, boost::asio::placeholders::error, 10 ));
}

void WeatherService::RunWeatherSequenceTimer(
	const boost::system::error_code& e, uint32_t count)
{
	tickPlanetWeather_();
	weather_timer_->expires_at(weather_timer_->expires_at() + boost::posix_time::seconds(10));
	weather_timer_->async_wait(boost::bind(&WeatherService::RunWeatherSequenceTimer, this, boost::asio::placeholders::error, 10 ));
}

void WeatherService::WeatherScript()
{
	try
    {
        script_.SetContext("kernel", boost::python::ptr(kernel_));
        script_.Run();
    }
    catch (std::exception& e)
	{
        LOG(fatal) << e.what();
	}
}

void WeatherService::Startup() 
{
	galaxy_service_  = kernel_->GetServiceManager()->GetService<GalaxyServiceInterface>("GalaxyService");
	weatherScriptTimer=0;
	RunWeatherSequence();
}

void WeatherService::Shutdown()
{
	sceneLookup_.clear();
	weather_sequence_.clear();
	weather_timer_->cancel();	
}

