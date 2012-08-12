#ifndef SWGANH_CONTAINER_INTERFACE_H_
#define SWGANH_CONTAINER_INTERFACE_H_

#include <memory>
#include <functional>

#include <anh/observer/observer_interface.h>

namespace swganh
{
namespace object
{
	class Object;

	class ContainerInterface
	{
	public:
		
		virtual uint64_t GetObjectId() = 0;

		//Object Management
		virtual void AddObject(std::shared_ptr<Object> newObject, int32_t arrangement_id=-2) = 0;

		virtual void RemoveObject(std::shared_ptr<Object> oldObject) = 0;

		virtual void TransferObject(std::shared_ptr<Object> object, std::shared_ptr<ContainerInterface> newContainer, int32_t arrangement_id=-2) = 0;

		virtual void SwapSlots(std::shared_ptr<Object> object, int32_t new_arrangement_id) {}

		virtual void ViewObjects(uint32_t max_depth, bool topDown, std::function<void(std::shared_ptr<Object>)> func, std::shared_ptr<Object> hint=nullptr) = 0;

		//FOR USE BY TRANSFER OBJECT ONLY. DO NOT CALL IN OUTSIDE CODE
		virtual void __InternalInsert(std::shared_ptr<Object> object, int32_t arrangement_id=-2) = 0;

		//Call to Create
		virtual void AddAwareObject(std::shared_ptr<swganh::object::Object> observer) {};
		
		//Call to View
		virtual void ViewAwareObjects(std::function<void(std::shared_ptr<swganh::object::Object>)> func) {};

		//Call to Destroy
		virtual void RemoveAwareObject(std::shared_ptr<swganh::object::Object> observer) {};

		virtual void LockObjectMutex() = 0;
		virtual void UnlockObjectMutex() = 0;
	};
}
}

#endif