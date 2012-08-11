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
		virtual void AddObject(std::shared_ptr<Object> newObject) = 0;
		virtual void RemoveObject(std::shared_ptr<Object> oldObject) = 0;
		virtual void TransferObject(std::shared_ptr<Object> object, std::shared_ptr<ContainerInterface> newContainer) = 0;

		virtual void ViewObjects(uint32_t max_depth, bool topDown, std::function<void(std::shared_ptr<Object>)> func, std::shared_ptr<Object> hint=nullptr) = 0;

		//FOR USE BY TRANSFER OBJECT ONLY. DO NOT CALL IN OUTSIDE CODE
		virtual void __InternalInsert(std::shared_ptr<Object> object) = 0;

		//Call to Create
		virtual void AddAwareObject(std::shared_ptr<anh::observer::ObserverInterface> observer) {};
		
		//Call to View
		virtual void ViewAwareObjects(std::function<void(std::shared_ptr<anh::observer::ObserverInterface>)> func) {};

		//Call to Destroy
		virtual void RemoveAwareObject(std::shared_ptr<anh::observer::ObserverInterface> observer) {};
	};
}
}

#endif