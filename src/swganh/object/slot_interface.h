#ifndef SLOT_INTERFACE_H_
#define SLOT_INTERFACE_H_

#include <memory>
#include <functional>

namespace swganh {
namespace object {

	class Object;

	class SlotInterface
	{
	public:
		virtual std::shared_ptr<swganh::object::Object> insert_object(std::shared_ptr<swganh::object::Object> insertObject) = 0;
		virtual void remove_object(std::shared_ptr<swganh::object::Object> removeObject) = 0;

		virtual bool is_filled() = 0;

		virtual void view_objects(std::function<void(std::shared_ptr<swganh::object::Object>&)> walkerFunction) = 0;
	};

}
}

#endif