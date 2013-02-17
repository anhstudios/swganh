// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <memory>
#include <boost/asio/deadline_timer.hpp>

#include "swganh/service/service_interface.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh_core/galaxy/providers/galaxy_provider_interface.h"

namespace swganh {
    class EventDispatcherInterface;
namespace app {
class KernelInterface;
}}  // namespace swganh::app


namespace swganh {
namespace galaxy {

    class GalaxyServiceInterface : public swganh::service::ServiceInterface
    {
    public:    

    	virtual uint32_t GetPopulation() = 0;
		
		virtual uint64_t GetGalaxyTimeInMilliseconds() = 0;

    	virtual swganh::service::ServiceDescription GetServiceDescription() = 0;
        
        virtual void Startup() = 0;

    };

}}  // swganh::galaxy
