
#ifndef WIN32
#include <Python.h>
#endif

#include "buff_manager.h"

#include <boost/asio/placeholders.hpp>

#include "swganh/event_dispatcher.h"
#include "swganh/logger.h"
#include "swganh/scripting/python_script.h"

#include "swganh_core/object/creature/creature.h"
#include "swganh_core/combat/buff_events.h"

using namespace swganh::scripting;
using namespace boost::posix_time;
using namespace swganh::app;
using namespace swganh::combat;
using namespace std;

BuffManager::BuffManager(swganh::app::SwganhKernel* kernel)
	: kernel_(kernel)
	, timer_(kernel_->GetCpuThreadPool())
{}

void BuffManager::Start()
{
	//Load Python Buffs
    PythonScript script(kernel_->GetAppConfig().script_directory + "/buffs/__init__.py");
    auto buff_names = script.GetGlobalAs<std::vector<std::string>>("buffTemplates");

    for (auto& buff_name : buff_names)
    {
        PythonScript script(kernel_->GetAppConfig().script_directory + "/buffs/" + buff_name + ".py");

		DLOG(info) << "Loading buff script " << buff_name;
        if(auto buff = script.CreateInstance<BuffInterface>(buff_name))
		{
			buff->filename = buff_name;
			buffs_.insert(std::make_pair(buff_name, buff));
		}
    }

	//Setup Dispatcher Callback
	kernel_->GetEventDispatcher()->Subscribe("CombatService::AddBuff", [this] (const shared_ptr<EventInterface>& incoming_event)
    {
		boost::lock_guard<boost::mutex> lock(this->mutex_);
        auto buff_event = static_pointer_cast<BuffEvent>(incoming_event);
		auto find_itr = buffs_.find(buff_event->name);
		if(find_itr != buffs_.end())
		{
			uint32_t duration;
			if(buff_event->duration == 0)
			{
				duration = find_itr->second->GetDuration();
			}
			else
			{
				duration = buff_event->duration;
			}

			ptime end_time = second_clock::local_time() + seconds(duration);
			q_.push(BuffQueueType(end_time, buff_event->object));
			buff_event->object->__AddBuffInternal(end_time, find_itr->second, duration);
		}
    });

	//Start the timer
	timer_.expires_from_now(boost::posix_time::seconds(1));
	timer_.async_wait(boost::bind(&BuffManager::handleTick_, this, boost::asio::placeholders::error));
}

void BuffManager::handleTick_(const boost::system::error_code& e)
{
	{
		ptime current_time = second_clock::local_time();

		boost::lock_guard<boost::mutex> lock(mutex_);
		while(!q_.empty() && current_time > q_.top().first)
		{
			q_.top().second->ClearBuffs(current_time);
			q_.pop();
		}
	}
	timer_.expires_from_now(boost::posix_time::seconds(1));
	timer_.async_wait(boost::bind(&BuffManager::handleTick_, this, boost::asio::placeholders::error));
}