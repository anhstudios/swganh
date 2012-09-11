#include "world_permission.h"

#include "swganh_core/object/object.h"
#include "swganh/object/container_interface.h"

using namespace swganh::object;

bool WorldPermission::canInsert(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object)
{
	return (requester == object);
}

bool WorldPermission::canRemove(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object)
{
	return (requester == object);
}

bool WorldPermission::canView(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester)
{
	return true;
}