#include "container_interface.h"
#include "swganh/object/permissions/container_permissions_interface.h"

#include <glm/glm.hpp>
#include <boost/thread/locks.hpp>

using namespace swganh::object;

boost::shared_mutex ContainerInterface::global_container_lock_;

std::shared_ptr<ContainerPermissionsInterface> ContainerInterface::GetPermissions() 
{ 
	return container_permissions_; 
}

void ContainerInterface::SetPermissions(std::shared_ptr<ContainerPermissionsInterface> obj) { 
	container_permissions_ = obj; 
}

void ContainerInterface::ViewObjects(std::shared_ptr<Object> requester, uint32_t max_depth, bool topDown, std::function<void(std::shared_ptr<Object>)> func)
{
	boost::shared_lock<boost::shared_mutex> shared(global_container_lock_);
	__InternalViewObjects(requester, max_depth, topDown, func);
}

void ContainerInterface::AddAwareObject(std::shared_ptr<swganh::object::Object> observer)
{
	boost::shared_lock<boost::shared_mutex> shared(global_container_lock_);
	__InternalAddAwareObject(observer);
}

void ContainerInterface::ViewAwareObjects(std::function<void(std::shared_ptr<swganh::object::Object>)> func, std::shared_ptr<swganh::object::Object> hint)
{
	boost::shared_lock<boost::shared_mutex> shared(global_container_lock_);
	__InternalViewAwareObjects(func, hint);
}

void ContainerInterface::RemoveAwareObject(std::shared_ptr<swganh::object::Object> observer)
{
	boost::shared_lock<boost::shared_mutex> shared(global_container_lock_);
	__InternalRemoveAwareObject(observer);
}

glm::vec3 ContainerInterface::GetAbsolutePosition()
{
	boost::shared_lock<boost::shared_mutex> shared(global_container_lock_);
	return __InternalGetAbsolutePosition();
}