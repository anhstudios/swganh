#include "world_cell_permission.h"

#include "swganh/object/container_interface.h"

using namespace swganh::object;

bool WorldCellPermission::canInsert(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object)
{
	return (requester == object);
}

bool WorldCellPermission::canRemove(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object)
{
	return (requester == object);
}

bool WorldCellPermission::canView(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester)
{
	return true;
}