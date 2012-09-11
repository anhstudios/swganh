// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh/galaxy/galaxy_service_interface.h"

namespace swganh {
namespace galaxy {

    class GalaxyService : public swganh::galaxy::GalaxyServiceInterface
    {
    public:    
        explicit GalaxyService(swganh::app::SwganhKernel* kernel);
        ~GalaxyService();

    	uint32_t GetPopulation();
		
		uint64_t GetGalaxyTimeInMilliseconds();

    	swganh::service::ServiceDescription GetServiceDescription();
        
        void Startup();

    private:
        GalaxyService();
        
        void GalaxyStatusTimerHandler_(const boost::system::error_code& e, int delay_in_secs);

    	std::shared_ptr<swganh::galaxy::providers::GalaxyProviderInterface> galaxy_provider_;
        swganh::app::SwganhKernel* kernel_;
        swganh::service::ServiceDirectoryInterface* service_directory_;
        swganh::EventDispatcherInterface* event_dispatcher_;
        std::shared_ptr<boost::asio::deadline_timer> galaxy_timer_;
    };

}}  // swganh::galaxy

