// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <memory>
#include <functional>

namespace swganh {
namespace object {

	class Object;

	class SlotInterface
	{
	public:
		virtual ~SlotInterface() {}
		virtual std::shared_ptr<swganh::object::Object> insert_object(const std::shared_ptr<swganh::object::Object> insertObject) = 0;
		virtual void remove_object(const std::shared_ptr<swganh::object::Object> removeObject) = 0;
		virtual void view_objects(std::function<void(const std::shared_ptr<swganh::object::Object>&)> walkerFunction) = 0;
		virtual void view_objects_if(std::function<bool(std::shared_ptr<swganh::object::Object>)> walkerFunction) = 0;
		virtual bool is_filled() = 0;
	};

}
}