#include "ridable_permission.h"

#include "swganh/object/container_interface.h"

using namespace swganh::object;

bool RideablePermission::canInsert(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object)
{
	return false;
}

bool RideablePermission::canRemove(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object)
{
	return false;
}

bool RideablePermission::canView(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester)
{
	return false;
}