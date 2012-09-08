#pragma once

#include "swganh/object/permissions/container_permissions_interface.h"

namespace swganh
{
namespace object
{

class StaticContainerPermission : public ContainerPermissionsInterface
{
	virtual PermissionType GetType()
	{
		return STATIC_CONTAINER_PERMISSION;
	}

	bool canInsert(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object);
	bool canRemove(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object);
	bool canView(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester);
};

}
}