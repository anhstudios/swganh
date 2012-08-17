// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_GALAXY_GALAXY_SERVICE_H_
#define SWGANH_GALAXY_GALAXY_SERVICE_H_

#include "swganh/galaxy/galaxy_service_interface.h"

namespace swganh_core {
namespace galaxy {

    class GalaxyService : public swganh::galaxy::GalaxyServiceInterface
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

    	std::shared_ptr<swganh::galaxy::providers::GalaxyProviderInterface> galaxy_provider_;
        swganh::app::SwganhKernel* kernel_;
        anh::service::ServiceDirectoryInterface* service_directory_;
        anh::EventDispatcherInterface* event_dispatcher_;
        std::shared_ptr<boost::asio::deadline_timer> galaxy_timer_;
    };

}}  // swganh::galaxy

#endif // SWGANH_GALAXY_GALAXY_SERVICE_H_
