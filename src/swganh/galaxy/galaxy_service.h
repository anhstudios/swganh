// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_GALAXY_GALAXY_SERVICE_H_
#define SWGANH_GALAXY_GALAXY_SERVICE_H_

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

    class GalaxyService : public anh::service::ServiceInterface
    {
    public:    
        explicit GalaxyService(swganh::app::SwganhKernel* kernel);
        ~GalaxyService();

    	uint32_t GetPopulation();
		
		uint64_t GetGalaxyTimeInMilliseconds();

    	anh::service::ServiceDescription GetServiceDescription();
        
        void Startup();

    private:
        GalaxyService();
        
        void GalaxyStatusTimerHandler_(const boost::system::error_code& e, int delay_in_secs);

    	std::shared_ptr<providers::GalaxyProviderInterface> galaxy_provider_;
        swganh::app::SwganhKernel* kernel_;
        anh::service::ServiceDirectoryInterface* service_directory_;
        anh::EventDispatcherInterface* event_dispatcher_;
        std::shared_ptr<boost::asio::deadline_timer> galaxy_timer_;
    };

}}  // swganh::galaxy

#endif // SWGANH_GALAXY_GALAXY_SERVICE_H_
