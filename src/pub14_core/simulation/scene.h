// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef PUB14_CORE_SIMULATION_SCENE_H_
#define PUB14_CORE_SIMULATION_SCENE_H_

#include "swganh/simulation/scene_interface.h"
#include "swganh/simulation/spatial_provider_interface.h"
#include <cstdint>
#include <string>

namespace swganh {
namespace app {
	class SwganhKernel;
}}

namespace swganh_core {
namespace simulation {
	class SpatialProviderInterface;

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
        Scene(SceneDescription description, swganh::app::SwganhKernel* kernel);
        Scene(
            uint32_t id,
            std::string name,
            std::string label,
            std::string description,
            std::string terrain,
			swganh::app::SwganhKernel* kernel);

        uint32_t GetSceneId() const;
        const std::string& GetName() const;
        const std::string& GetLabel() const;
        const std::string& GetDescription() const;
		const std::string& GetTerrainMap() const;

        void AddObject(std::shared_ptr<swganh::object::Object> object);

        void RemoveObject(std::shared_ptr<swganh::object::Object> object);

		void HandleDataTransform(const std::shared_ptr<swganh::object::ObjectController>& controller, swganh::messages::controllers::DataTransform message);
		void HandleDataTransformWithParent(const std::shared_ptr<swganh::object::ObjectController>& controller, swganh::messages::controllers::DataTransformWithParent message);

    private:
        Scene();

        class SceneImpl;
        std::shared_ptr<SceneImpl> impl_;
    };

}}  // namespace swganh::simulation

#endif  // PUB14_CORE_SIMULATION_SCENE_H_
