// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_GALAXY_GALAXY_SERVICE_INTERFACE_H_
#define SWGANH_GALAXY_GALAXY_SERVICE_INTERFACE_H_

#include <memory>
#include <boost/asio/deadline_timer.hpp>

#include "anh/service/service_interface.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh/galaxy/providers/galaxy_provider_interface.h"

namespace anh {
    class EventDispatcherInterface;
namespace app {
class KernelInterface;
}}  // namespace anh::app


namespace swganh {
namespace galaxy {

    class GalaxyServiceInterface : public anh::service::ServiceInterface
    {
    public:    

    	virtual uint32_t GetPopulation() = 0;
		
		virtual uint64_t GetGalaxyTimeInMilliseconds() = 0;

    	virtual anh::service::ServiceDescription GetServiceDescription() = 0;
        
        virtual void Startup() = 0;

    };

}}  // swganh::galaxy

#endif // SWGANH_GALAXY_GALAXY_SERVICE_H_
