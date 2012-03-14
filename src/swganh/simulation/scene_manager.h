
#ifndef SWGANH_SIMULATION_SCENE_MANAGER_H_
#define SWGANH_SIMULATION_SCENE_MANAGER_H_

#include <cstdint>
#include <map>
#include <memory>
#include <string>

#include "swganh/simulation/scene.h"

namespace sql {
    class Connection;
}  // namespace sql

namespace swganh {
namespace simulation {
    
    class SceneManager
    {
    public:

        void LoadSceneDescriptionsFromDatabase(const std::shared_ptr<sql::Connection>& connection);
        
        std::shared_ptr<Scene> GetScene(const std::string& scene_label) const;
		std::shared_ptr<Scene> GetScene(uint32_t scene_id) const;

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

}}  // namespace swganh::simulation

#endif  // SWGANH_SIMULATION_SCENE_MANAGER_H_
