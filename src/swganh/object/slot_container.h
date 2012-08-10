#ifndef SLOT_CONTAINER_H_
#define SLOT_CONTAINER_H_

#include "slot_interface.h"

#include <set>

namespace swganh {
namespace object {

	class SlotContainer : public SlotInterface
	{
	public:

		virtual std::shared_ptr<swganh::object::Object> insert_object(std::shared_ptr<swganh::object::Object> insertObject);
		virtual void remove_object(std::shared_ptr<swganh::object::Object> removeObject);
		virtual void view_objects(std::function<void(std::shared_ptr<swganh::object::Object>&)> walkerFunction);

	private:
		std::set<std::shared_ptr<swganh::object::Object>> held_objects_;
	};

}
}

#endif