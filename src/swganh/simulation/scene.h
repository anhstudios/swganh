
#ifndef SWGANH_SIMULATION_SCENE_H_
#define SWGANH_SIMULATION_SCENE_H_

#include <cstdint>
#include <map>
#include <memory>
#include <set>
#include <string>

#include <boost/noncopyable.hpp>

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

    class Scene : boost::noncopyable
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

#endif  // SWGANH_SIMULATION_SCENE_H_
