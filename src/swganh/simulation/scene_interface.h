// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

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

    class SceneInterface : boost::noncopyable
    {
	public:
        virtual uint32_t GetSceneId() const = 0;
        virtual const std::string& GetName() const = 0;
        virtual const std::string& GetLabel() const = 0;
        virtual const std::string& GetDescription() const = 0;
		virtual const std::string& GetTerrainMap() const =  0;

        virtual void AddObject(const std::shared_ptr<swganh::object::Object>& object) = 0;

        virtual void RemoveObject(const std::shared_ptr<swganh::object::Object>& object) = 0;
    };

}}  // namespace swganh::simulation

#endif  // SWGANH_SIMULATION_SCENE_H_
