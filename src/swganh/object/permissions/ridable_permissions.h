#pragma once

#include "container_permissions_interface.h"

namespace swganh
{
namespace object
{

class RideablePermissions : public ContainerPermissionsInterface
{
	virtual PermissionType GetType()
	{
		return RIDEABLE_CONTAINER_PERMISSION;
	}

	bool canInsert(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object);
	bool canRemove(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object);
	bool canView(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester);
};

}
}
