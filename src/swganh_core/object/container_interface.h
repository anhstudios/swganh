// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#pragma once

#include <list>
#include <memory>
#include <functional>

#include <swganh/observer/observer_interface.h>
#include <boost/thread/shared_mutex.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace swganh
{
namespace object
{
	class Object;
	class ContainerPermissionsInterface;

	class ContainerInterface
	{
	public:
		
		virtual ~ContainerInterface() {}
		
		virtual uint64_t GetObjectId() = 0;

		//Object Management
		virtual void AddObject(std::shared_ptr<Object> requester, std::shared_ptr<Object> newObject, int32_t arrangement_id=-2) = 0;
		virtual void RemoveObject(std::shared_ptr<Object> requester, std::shared_ptr<Object> oldObject) = 0;
		//virtual void TransferObject(std::shared_ptr<Object> requester, std::shared_ptr<Object> object, std::shared_ptr<ContainerInterface> newContainer, int32_t arrangement_id=-2);
		virtual void TransferObject(std::shared_ptr<Object> requester, std::shared_ptr<Object> object, std::shared_ptr<ContainerInterface> newContainer, glm::vec3 position, int32_t arrangement_id=-2) = 0;
		virtual void SwapSlots(std::shared_ptr<Object> requester, std::shared_ptr<Object> object, int32_t new_arrangement_id) {};

		bool HasContainedObjects();
		bool __InternalHasContainedObjects();

		std::list<std::shared_ptr<Object>> GetObjects(std::shared_ptr<Object> requester, uint32_t max_depth, bool topDown);
		void GetObjects(std::shared_ptr<Object> requester, uint32_t max_depth, bool topDown, std::list<std::shared_ptr<Object>>& out);
		virtual void __InternalGetObjects(std::shared_ptr<Object> requester, uint32_t max_depth, bool topDown, std::list<std::shared_ptr<Object>>& out) = 0;

		void ViewObjects(std::shared_ptr<Object> requester, uint32_t max_depth, bool topDown, std::function<void(std::shared_ptr<Object>)> func);
		virtual void __InternalViewObjects(std::shared_ptr<Object> requester, uint32_t max_depth, bool topDown, std::function<void(std::shared_ptr<Object>)> func) = 0;

		virtual std::shared_ptr<ContainerPermissionsInterface> GetPermissions();
		virtual void SetPermissions(std::shared_ptr<ContainerPermissionsInterface> obj);

		//FOR USE BY TRANSFER OBJECT ONLY. DO NOT CALL IN OUTSIDE CODE
		virtual void __InternalTransfer(std::shared_ptr<Object> requester, std::shared_ptr<Object> object, std::shared_ptr<ContainerInterface> newContainer, int32_t arrangement_id=-2) {};
		virtual int32_t __InternalInsert(std::shared_ptr<Object> object, glm::vec3 new_position, int32_t arrangement_id=-2) = 0;
		
		//Call to Create
		void AddAwareObject(std::shared_ptr<swganh::object::Object> observer);
		virtual void __InternalAddAwareObject(std::shared_ptr<swganh::object::Object> observer, bool reverse_still_valid) {};

		//Call to View
		void ViewAwareObjects(std::function<void(std::shared_ptr<swganh::object::Object>)> func, std::shared_ptr<swganh::object::Object> hint=nullptr);
		virtual void __InternalViewAwareObjects(std::function<void(std::shared_ptr<swganh::object::Object>)> func, std::shared_ptr<swganh::object::Object> hint=nullptr) = 0;

		//Call to Destroy
		void RemoveAwareObject(std::shared_ptr<swganh::object::Object> observer);
		virtual void __InternalRemoveAwareObject(std::shared_ptr<swganh::object::Object> observer, bool reverse_still_valid) {};

		virtual std::shared_ptr<ContainerInterface> GetContainer() = 0;
		virtual void SetContainer(const std::shared_ptr<ContainerInterface>& container) = 0;

		virtual void GetAbsolutes(glm::vec3& pos, glm::quat& rot);
		virtual void __InternalGetAbsolutes(glm::vec3& pos, glm::quat& rot) = 0;

	protected:
		std::shared_ptr<swganh::object::ContainerPermissionsInterface> container_permissions_;

		static boost::shared_mutex global_container_lock_;
	};
}
}