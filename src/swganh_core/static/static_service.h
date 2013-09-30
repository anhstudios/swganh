// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#pragma once

#include <map>
#include <vector>

#include "swganh/active_object.h"
#include "swganh/app/swganh_kernel.h"
#include "swganh_core/static/static_service_interface.h"
#include "swganh_core/static/skill_manager.h"

namespace sql
{
class ResultSet;
}

namespace swganh
{
class EventInterface;

namespace simulation
{
class SimulationServiceInterface;
}

namespace spawn
{
class SpawnServiceInterface;
}

namespace object
{
class Creature;
}

namespace statics
{
class StaticService : public swganh::statics::StaticServiceInterface
{
public:

    StaticService(swganh::app::SwganhKernel* kernel);
    ~StaticService();

    void Initialize();
    void Startup();

    std::vector<std::shared_ptr<ElevatorData>> GetElevatorDataForObject(uint64_t terminal_id);

    /*
     * @brief Gets a given skill mod and any affected Attributes if exist
     * @return a pair of base, modifier
     */
    std::pair<uint32_t, uint32_t> GetSkillMod(const std::shared_ptr<swganh::object::Creature>& creature, const std::string& skill_mod_name);

    /*
     * @brief Gets All SkillMods that are applicable for this creature.
     * @return a map of pairs of base, modifier
     */
    std::map<std::string, std::pair<uint32_t, uint32_t>> GetSkillMods(const std::shared_ptr<swganh::object::Creature>& creature);

private:

    void _loadBuildings(swganh::simulation::SimulationServiceInterface* simulation_service, std::unique_ptr<sql::ResultSet> result,
                        uint32_t scene_id, std::string scene_name);
    void _loadCells(swganh::simulation::SimulationServiceInterface* simulation_service, std::unique_ptr<sql::ResultSet> result,
                    uint32_t scene_id, std::string scene_name);
    void _loadCloneLocations(swganh::simulation::SimulationServiceInterface* simulation_service, std::unique_ptr<sql::ResultSet> result,
                             uint32_t scene_id, std::string scene_name);
    void _loadTerminals(swganh::simulation::SimulationServiceInterface* simulation_service, std::unique_ptr<sql::ResultSet> result,
                        uint32_t scene_id, std::string scene_name);
    void _loadElevatorData(swganh::simulation::SimulationServiceInterface* simulation_service, std::unique_ptr<sql::ResultSet> result,
                           uint32_t scene_id, std::string scene_name);
    void _loadContainers(swganh::simulation::SimulationServiceInterface* simulation_service, std::unique_ptr<sql::ResultSet> result,
                         uint32_t scene_id, std::string scene_name);
    void _loadTicketCollectors(swganh::simulation::SimulationServiceInterface* simulation_service, std::unique_ptr<sql::ResultSet> result,
                               uint32_t scene_id, std::string scene_name);
    void _loadNPCS(swganh::simulation::SimulationServiceInterface* simulation_service, swganh::spawn::SpawnServiceInterface* spawn_service,
                   std::unique_ptr<sql::ResultSet> result, uint32_t scene_id, std::string scene_name);
    void _loadShuttles(swganh::simulation::SimulationServiceInterface* simulation_service, swganh::spawn::SpawnServiceInterface* spawn_service,
                       std::unique_ptr<sql::ResultSet> result, uint32_t scene_id, std::string scene_name);

    swganh::app::SwganhKernel* kernel_;

    std::map<uint64_t, std::vector<std::shared_ptr<ElevatorData>>> elevator_lookup_;
    SkillManager skill_mod_manager_;
    swganh::ActiveObject active_;
};
}
}