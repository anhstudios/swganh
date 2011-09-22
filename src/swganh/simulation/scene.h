
#ifndef SWGANH_SIMULATION_SCENE_H_
#define SWGANH_SIMULATION_SCENE_H_

#include <cstdint>
#include <memory>
#include <string>

namespace swganh {
namespace object {
    class Object;
}}  // namespace swganh::object

namespace swganh {
namespace simulation {

    struct SceneDescription
    {
        uint32_t id;
        std::string name;
        std::string label;
        std::string description;
        std::string terrain;
    };

    class Scene
    {
    public:
        explicit Scene(const SceneDescription& description);
        Scene(
            std::string name,
            std::string label,
            std::string description,
            std::string terrain);

        void AddObject(const std::shared_ptr<swganh::object::Object>& object);

        void RemoveObject(const std::shared_ptr<swganh::object::Object>& object);
    };

}}  // namespace swganh::simulation

#endif  // SWGANH_SIMULATION_SCENE_H_
