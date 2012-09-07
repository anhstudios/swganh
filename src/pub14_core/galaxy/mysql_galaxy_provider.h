// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh/galaxy/providers/galaxy_provider_interface.h"
#include <memory>

namespace swganh { namespace database { class DatabaseManagerInterface; 
}}  // anh::database

namespace swganh_core {
namespace galaxy {

class MysqlGalaxyProvider : public swganh::galaxy::providers::GalaxyProviderInterface {
public:
    explicit MysqlGalaxyProvider(anh::database::DatabaseManagerInterface* db_manager);
    ~MysqlGalaxyProvider();

	virtual uint32_t GetPopulation();
	
private:
    anh::database::DatabaseManagerInterface* db_manager_;
};

}}  // namespace swganh::galaxy::providers
