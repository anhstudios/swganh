// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "command_properties_tre_loader.h"

#include <exception>

#include "anh/logger.h"

#include "swganh/tre/tre_archive.h"

using anh::HashString;
using pub14_core::command::CommandPropertiesTreLoader;
using swganh::command::CommandProperties;
using swganh::command::CommandPropertiesMap;
using swganh::tre::readers::DatatableReader;
using std::exception;
using std::make_pair;
using std::string;
using std::vector;

CommandPropertiesTreLoader::CommandPropertiesTreLoader(swganh::tre::TreArchive* archive)
    : archive_(archive)
{}

CommandPropertiesMap CommandPropertiesTreLoader::LoadCommandPropertiesMap()
{
    CommandPropertiesMap properties_map;
    
    try 
    {
        DatatableReader reader(archive_->GetResource("datatables/command/command_table.iff"));
        
        while(reader.Next())
        {
            auto row = reader.GetRow();

            CommandProperties properties;

            auto tmp_command_name = row["commandName"]->GetValue<string>();            
            std::transform(tmp_command_name.begin(), tmp_command_name.end(), tmp_command_name.begin(), ::tolower);

            properties.command_name = HashString(tmp_command_name);
            properties.default_priority = row["defaultPriority"]->GetValue<int>();
            properties.default_time = row["defaultTime"]->GetValue<float>();
            properties.character_ability = HashString(row["characterAbility"]->GetValue<string>());
            properties.target_type = row["targetType"]->GetValue<int>();
            properties.call_on_target = row["callOnTarget"]->GetValue<int>();
            properties.command_group = row["commandGroup"]->GetValue<int>();
            properties.max_range_to_target = row["maxRangeToTarget"]->GetValue<float>();
            properties.god_level = row["godLevel"]->GetValue<int>();
            properties.add_to_combat_queue = row["addToCombatQueue"]->GetValue<int>();
            
            properties.allow_in_locomotion = BuildAllowInLocomotion(&row);
            properties.allow_in_locomotion = BuildAllowInState(&row);

            properties_map.insert(make_pair(properties.command_name.ident(), properties));
        }
    }
    catch(exception& e)
    {
        LOG(error) << e.what();
    }
    
    return properties_map;
}

uint64_t CommandPropertiesTreLoader::BuildAllowInLocomotion(swganh::tre::readers::DatatableRow* row)
{    
    vector<int> bits;

    bits.push_back((*row)["L:standing"]->GetValue<int>());
    bits.push_back((*row)["L:sneaking"]->GetValue<int>());
    bits.push_back((*row)["L:sneaking"]->GetValue<int>());
    bits.push_back((*row)["L:walking"]->GetValue<int>());
    bits.push_back((*row)["L:running"]->GetValue<int>());
    bits.push_back((*row)["L:kneeling"]->GetValue<int>());
    bits.push_back((*row)["L:crouchSneaking"]->GetValue<int>());
    bits.push_back((*row)["L:crouchWalking"]->GetValue<int>());
    bits.push_back((*row)["L:prone"]->GetValue<int>());
    bits.push_back((*row)["L:crawling"]->GetValue<int>());
    bits.push_back((*row)["L:climbingStationary"]->GetValue<int>());
    bits.push_back((*row)["L:climbing"]->GetValue<int>());
    bits.push_back((*row)["L:hovering"]->GetValue<int>());
    bits.push_back((*row)["L:flying"]->GetValue<int>());
    bits.push_back((*row)["L:sitting"]->GetValue<int>());
    bits.push_back((*row)["L:skillAnimating"]->GetValue<int>());
    bits.push_back((*row)["L:drivingVehicle"]->GetValue<int>());
    bits.push_back((*row)["L:ridingCreature"]->GetValue<int>());
    bits.push_back((*row)["L:knockedDown"]->GetValue<int>());
    bits.push_back((*row)["L:incapacitated"]->GetValue<int>());
    bits.push_back((*row)["L:dead"]->GetValue<int>());
    bits.push_back((*row)["L:blocking"]->GetValue<int>());

    return BuildBitmask(bits);
}

uint64_t CommandPropertiesTreLoader::BuildAllowInState(swganh::tre::readers::DatatableRow* row)
{
    vector<int> bits;

    bits.push_back((*row)["S:cover"]->GetValue<int>());
    bits.push_back((*row)["S:combat"]->GetValue<int>());
    bits.push_back((*row)["S:peace"]->GetValue<int>());
    bits.push_back((*row)["S:aiming"]->GetValue<int>());
    bits.push_back((*row)["S:alert"]->GetValue<int>());
    bits.push_back((*row)["S:berserk"]->GetValue<int>());
    bits.push_back((*row)["S:feignDeath"]->GetValue<int>());
    bits.push_back((*row)["S:combatAttitudeEvasive"]->GetValue<int>());
    bits.push_back((*row)["S:combatAttitudeNormal"]->GetValue<int>());
    bits.push_back((*row)["S:combatAttitudeAggressive"]->GetValue<int>());
    bits.push_back((*row)["S:tumbling"]->GetValue<int>());
    bits.push_back((*row)["S:rallied"]->GetValue<int>());
    bits.push_back((*row)["S:stunned"]->GetValue<int>());
    bits.push_back((*row)["S:blinded"]->GetValue<int>());
    bits.push_back((*row)["S:dizzy"]->GetValue<int>());
    bits.push_back((*row)["S:intimidated"]->GetValue<int>());
    bits.push_back((*row)["S:immobilized"]->GetValue<int>());
    bits.push_back((*row)["S:frozen"]->GetValue<int>());
    bits.push_back((*row)["S:swimming"]->GetValue<int>());
    bits.push_back((*row)["S:sittingOnChair"]->GetValue<int>());
    bits.push_back((*row)["S:crafting"]->GetValue<int>());
    bits.push_back((*row)["S:glowingJedi"]->GetValue<int>());
    bits.push_back((*row)["S:maskScent"]->GetValue<int>());
    bits.push_back((*row)["S:poisoned"]->GetValue<int>());
    bits.push_back((*row)["S:bleeding"]->GetValue<int>());
    bits.push_back((*row)["S:diseased"]->GetValue<int>());
    bits.push_back((*row)["S:onFire"]->GetValue<int>());
    bits.push_back((*row)["S:ridingMount"]->GetValue<int>());
    bits.push_back((*row)["S:mountedCreature"]->GetValue<int>());
    bits.push_back((*row)["S:pilotingShip"]->GetValue<int>());
    bits.push_back((*row)["S:pilotingPobShip"]->GetValue<int>());
    bits.push_back((*row)["S:shipOperations"]->GetValue<int>());
    bits.push_back((*row)["S:shipGunner"]->GetValue<int>());
    bits.push_back((*row)["S:shipInterior"]->GetValue<int>());

    return BuildBitmask(bits);
}

uint64_t CommandPropertiesTreLoader::BuildBitmask(const vector<int>& bits)
{
    uint64_t bitmask = 0;
    int counter = 0;
    for (auto& bit : bits)
    {
        bitmask += bit << counter++;
    }
    return bitmask;
}
