// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef PUB14_CORE_SIMULATION_SCENE_H_
#define PUB14_CORE_SIMULATION_SCENE_H_

#include "swganh/simulation/scene_interface.h"
#include <cstdint>
#include <string>

namespace swganh_core {
namespace simulation {

    struct SceneDescription
    {
        uint32_t id;
        std::string name;
        std::string label;
        std::string description;
        std::string terrain;
    };

    class Scene : public swganh::simulation::SceneInterface
    {
    public:
        explicit Scene(SceneDescription description);
        Scene(
            uint32_t id,
            std::string name,
            std::string label,
            std::string description,
            std::string terrain);

        uint32_t GetSceneId() const;
        const std::string& GetName() const;
        const std::string& GetLabel() const;
        const std::string& GetDescription() const;
		const std::string& GetTerrainMap() const;

        void AddObject(const std::shared_ptr<swganh::object::Object>& object);

        void RemoveObject(const std::shared_ptr<swganh::object::Object>& object);

    private:
        Scene();

        class SceneImpl;
        std::shared_ptr<SceneImpl> impl_;
    };

}}  // namespace swganh::simulation

#endif  // PUB14_CORE_SIMULATION_SCENE_H_
