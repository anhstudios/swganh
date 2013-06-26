// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh_core/galaxy/galaxy_service_interface.h"

namespace swganh {
namespace galaxy {

/**
* Manages statistics on a galaxy and provides galaxy time
*/
class GalaxyService : public swganh::galaxy::GalaxyServiceInterface
{
public:    

	/**
	* Creates a new instance
	*/
    explicit GalaxyService(swganh::app::SwganhKernel* kernel);

	/**
	* Custom destructor
	*/
    ~GalaxyService();

	/**
	* @return the population of the galaxy
	*/
    uint32_t GetPopulation();
		
	/**
	* @return the current time of the galaxy since galaxy birth
	*/
	uint64_t GetGalaxyTimeInMilliseconds();
        
	/**
	* Called on server startup
	*/
    void Initialize();
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

