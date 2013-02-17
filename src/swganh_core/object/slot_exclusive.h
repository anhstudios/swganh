// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh_core/object/slot_interface.h"

namespace swganh {
namespace object {

	class SlotExclusive : public SlotInterface
	{
	public:

		virtual std::shared_ptr<swganh::object::Object> insert_object(const std::shared_ptr<swganh::object::Object> insertObject);
		virtual void remove_object(const std::shared_ptr<swganh::object::Object> removeObject);
		virtual bool is_filled() { return held_object_ != nullptr; }
		virtual void view_objects(std::function<void(const std::shared_ptr<swganh::object::Object>&)> walkerFunction);
		virtual void view_objects_if(std::function<bool(std::shared_ptr<swganh::object::Object>)> walkerFunction);

	private:
		std::shared_ptr<swganh::object::Object> held_object_;
	};

}
}
