#ifndef SWGANH_CREATURE_CONTAINER_PERMISSIONS_H_
#define SWGANH_CREATURE_CONTAINER_PERMISSIONS_H_

#include "container_permissions_interface.h"

namespace swganh
{
namespace object
{

class CreatureContainerPermissions : public ContainerPermissionsInterface
{
	virtual PermissionType GetType()
	{
		return CREATURE_CONTAINER_CONTAINER_PERMISSION;
	}

	bool canInsert(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object);
	bool canRemove(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object);
	bool canView(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester);
};

}
}

#endif