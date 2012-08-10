#ifndef SLOT_EXCLUSIVE_H_
#define SLOT_EXCLUSIVE_H_

#include "slot_interface.h"

namespace swganh {
namespace object {

	class SlotExclusive : public SlotInterface
	{
	public:

		virtual std::shared_ptr<swganh::object::Object> insert_object(std::shared_ptr<swganh::object::Object> insertObject);
		virtual void remove_object(std::shared_ptr<swganh::object::Object> removeObject);
		virtual bool is_filled() { return held_object_ != nullptr; }
		virtual void view_objects(std::function<void(std::shared_ptr<swganh::object::Object>&)> walkerFunction);

	private:
		std::shared_ptr<swganh::object::Object> held_object_;
	};

}
}

#endif