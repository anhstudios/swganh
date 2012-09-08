// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "command_properties_manager.h"

#include <exception>

#include "swganh/logger.h"

#include "swganh/tre/resource_manager.h"
#include "swganh/tre/visitors/visitor_types.h"
#include "swganh/tre/visitors/datatables/datatable_visitor.h"

using namespace swganh::tre;

using swganh::HashString;
using swganh::command::CommandPropertiesManager;
using swganh::command::CommandProperties;
using swganh::command::CommandPropertiesMap;
using std::exception;
using std::make_pair;
using std::string;
using std::vector;

CommandPropertiesManager::CommandPropertiesManager(swganh::tre::ResourceManager* resource_manager)
    : resource_manager_(resource_manager)
{
    command_properties_map_ = LoadCommandPropertiesMap();
}

CommandPropertiesManager::~CommandPropertiesManager()
{
    
}

boost::optional<const CommandProperties&> CommandPropertiesManager::FindPropertiesForCommand(HashString command)
{
    boost::optional<const CommandProperties&> command_properties;
    
    auto find_iter = command_properties_map_.find(command);
    if (find_iter != command_properties_map_.end())
    {
        command_properties = find_iter->second;
    }

    return command_properties;
}

CommandPropertiesMap CommandPropertiesManager::LoadCommandPropertiesMap()
{
    CommandPropertiesMap properties_map;
    
    try 
    {
		auto datatable = resource_manager_->GetResourceByName<DatatableVisitor>("datatables/command/command_table.iff");
        
		vector<int> bits;
        std::for_each(datatable->begin_itr(), datatable->end_itr(), [&] (DatatableVisitor::DATA_ROW& row) {

            CommandProperties properties;

            auto tmp_command_name = row.GetValue<string>(0);            
            std::transform(tmp_command_name.begin(), tmp_command_name.end(), tmp_command_name.begin(), ::tolower);

            properties.command_name = HashString(tmp_command_name);
            properties.default_priority = row.GetValue<uint32_t>(1);
            properties.default_time = row.GetValue<float>(6);
            properties.character_ability = HashString(row.GetValue<string>(7));

			//Locomotion
			for(int i=8; i <= 29; ++i) {
				bits.push_back(row.GetValue<uint32_t>(i));
			}
			properties.allow_in_locomotion = BuildBitmask(bits);
			bits.clear();

			//State
			for(int i=30; i <= 63; ++i) {
				bits.push_back(row.GetValue<uint32_t>(i));
			}
			properties.allow_in_state = BuildBitmask(bits);

            properties.target_type = row.GetValue<uint32_t>(65);
            properties.call_on_target = row.GetValue<uint32_t>(68);
            properties.command_group = row.GetValue<uint32_t>(69);
            properties.max_range_to_target = row.GetValue<float>(71);
            properties.god_level = row.GetValue<uint32_t>(72);
            properties.add_to_combat_queue = row.GetValue<uint32_t>(74);

            properties_map.insert(make_pair(properties.command_name, properties));
        });
    }
    catch(exception& e)
    {
        LOG(error) << e.what();
    }
    
    return properties_map;
}

uint64_t CommandPropertiesManager::BuildBitmask(const vector<int>& bits) const
{
    uint64_t bitmask = 0;
    int counter = 0;
    for (auto& bit : bits)
    {
        bitmask += bit << counter++;
    }
    return bitmask;
}
