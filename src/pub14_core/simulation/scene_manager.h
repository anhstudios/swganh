// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef PUB14_CORE_SIMULATION_SCENE_MANAGER_H_
#define PUB14_CORE_SIMULATION_SCENE_MANAGER_H_

#include <cstdint>
#include <map>
#include <memory>
#include <string>

#include "scene.h"
#include "swganh/simulation/scene_manager_interface.h"

namespace swganh_core {
namespace simulation {
    
    class SceneManager : public swganh::simulation::SceneManagerInterface
    {
    public:

        void LoadSceneDescriptionsFromDatabase(const std::shared_ptr<sql::Connection>& connection);
        
        std::shared_ptr<swganh::simulation::SceneInterface> GetScene(const std::string& scene_label) const;
		std::shared_ptr<swganh::simulation::SceneInterface> GetScene(uint32_t scene_id) const;

        void StartScene(const std::string& scene_label);
        void StopScene(const std::string& scene_label);

    private:
        typedef std::map<
            std::string,
            SceneDescription
        > SceneDescriptionMap;        

        typedef std::map<
            std::string,
            std::shared_ptr<Scene>
        > SceneMap;

		typedef std::pair<
			std::string,
			std::shared_ptr<Scene>
		> ScenePair;

        SceneDescriptionMap scene_descriptions_;
        SceneMap scenes_;
    };

}}  // namespace swganh_core::simulation

#endif  // PUB14_CORE_SIMULATION_SCENE_MANAGER_H_
