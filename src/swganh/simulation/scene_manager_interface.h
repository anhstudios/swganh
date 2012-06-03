// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_SIMULATION_SCENE_MANAGER_INTERFACE_H_
#define SWGANH_SIMULATION_SCENE_MANAGER_INTERFACE_H_

#include <cstdint>
#include <map>
#include <memory>
#include <string>

#include "swganh/simulation/scene_interface.h"

namespace sql {
    class Connection;
}  // namespace sql

namespace swganh {
namespace simulation {
    
    class SceneManagerInterface
    {
    public:

        virtual void LoadSceneDescriptionsFromDatabase(const std::shared_ptr<sql::Connection>& connection) = 0;
        
        virtual std::shared_ptr<SceneInterface> GetScene(const std::string& scene_label) const = 0;
		virtual std::shared_ptr<SceneInterface> GetScene(uint32_t scene_id) const = 0;

        virtual void StartScene(const std::string& scene_label) = 0;
        virtual void StopScene(const std::string& scene_label) = 0;
    };

}}  // namespace swganh::simulation

#endif  // SWGANH_SIMULATION_SCENE_MANAGER_INTERFACE_H_
