// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <functional>
#include <cstdint>
#include <map>
#include <memory>
#include <set>
#include <string>

#include <boost/noncopyable.hpp>

namespace swganh {
namespace observer {
	class ObserverInterface;
}}

namespace swganh {
namespace object {
    class Object;
}}  // namespace swganh::object

namespace swganh {
namespace messages {
namespace controllers {
	class DataTransform;
	class DataTransformWithParent;
}}}

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

        virtual void AddObject(std::shared_ptr<swganh::object::Object> object) = 0;
        virtual void RemoveObject(std::shared_ptr<swganh::object::Object> object) = 0;
		virtual void ViewObjects(std::shared_ptr<swganh::object::Object> requester, uint32_t max_depth, 
			bool topDown, std::function<void(std::shared_ptr<swganh::object::Object>)> func) = 0;

		virtual void HandleDataTransform(const std::shared_ptr<swganh::object::Object>& controller, swganh::messages::controllers::DataTransform message) = 0;
		virtual void HandleDataTransformWithParent(const std::shared_ptr<swganh::object::Object>& controller, swganh::messages::controllers::DataTransformWithParent message) = 0;
    };

}}  // namespace swganh::simulation
