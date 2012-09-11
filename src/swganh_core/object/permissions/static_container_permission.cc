#include "static_container_permission.h"

#include "swganh_core/object/object.h"
#include "swganh/object/container_interface.h"

using namespace swganh::object;

bool StaticContainerPermission::canInsert(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object)
{
	return false;
}

bool StaticContainerPermission::canRemove(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object)
{
	return true;
}

bool StaticContainerPermission::canView(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester)
{
	return true;
}