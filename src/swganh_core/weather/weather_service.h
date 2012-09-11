// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_CORE_WEATHER_WEATHER_SERVICE_H_
#define SWGANH_CORE_WEATHER_WEATHER_SERVICE_H_

#include <cstdint>
#include <map>
#include <memory>
#include <glm/glm.hpp>
#include "swganh_core/messages/server_weather_message.h"
#include "swganh_core/object/object.h"
#include "swganh/weather/weather_service_interface.h"
#include "swganh/app/swganh_kernel.h"
#include "swganh/weather/weather_data.h"
#include "swganh/event_dispatcher.h"
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include "swganh/galaxy/galaxy_service_interface.h"
#include "swganh/scripting/python_script.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh/observer/observer_interface.h"
#include "swganh/player/player_service_interface.h"


namespace swganh {
namespace simulation {
	class SimulationServiceInterface;
}} // namespace swganh::simulation

namespace swganh {
namespace weather {

	class PlanetWeather
	{
	public:
		uint64_t updateGalaxyTickTime_;
		std::vector<swganh::weather::WeatherEvent> weatherSequence_;
		swganh::weather::Scene scene_;
		uint32_t sequenceCount_;
	};
    
    class WeatherService : public swganh::weather::WeatherServiceInterface
    {
    public:
        explicit WeatherService(swganh::app::SwganhKernel* kernel);
    
        ~WeatherService();

        swganh::service::ServiceDescription GetServiceDescription();
		
		swganh::weather::Weather GetSceneWeather(uint32_t scene_id);
		void SetSceneWeather(uint32_t scene_id,  std::vector<swganh::weather::WeatherEvent> weather_sequence_);
		void swganh::weather::WeatherService::RunWeatherSequence();
		void Startup();

    private:
		void WeatherService::tickPlanetWeather_();
		void WeatherService::WeatherScript();
		void WeatherService::RemoveWeatherScene(uint32_t scene_);
		void SendServerWeatherMessagePlayer_(swganh::weather::WeatherEvent weatherdata_, std::shared_ptr<swganh::observer::ObserverInterface> obj_controller);
		void SendServerWeatherMessageAll_(swganh::weather::Weather weather_type, glm::vec3 cloud_vector, uint32_t scene_id);
		void OnPlayerEnter(std::shared_ptr<swganh::object::Object> player_obj);
		void swganh::weather::WeatherService::RunWeatherSequenceTimer(const boost::system::error_code& e, uint32_t count);

		swganh::app::SwganhKernel* kernel_;
		swganh::scripting::PythonScript script_;
		swganh::simulation::SimulationServiceInterface* simulation_service_;
		swganh::CallbackId player_selected_callback_;
		swganh::galaxy::GalaxyServiceInterface* galaxy_service_;
		mutable boost::mutex weather_mutex_;
		std::vector<swganh::weather::WeatherEvent> weather_sequence_;
		std::shared_ptr<boost::asio::deadline_timer> weather_timer_;
		std::map<uint32_t, PlanetWeather> sceneLookup_;
		uint64_t weatherScriptTimer;
		std::string script_prefix_;
    };
}}  // namespace swganh::weather

#endif  // SWGANH_CORE_WEATHER_WEATHER_SERVICE_H_
