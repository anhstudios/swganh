#ifndef SWGANH_CREATURE_PERMISSIONS_H_
#define SWGANH_CREATURE_PERMISSIONS_H_

#include "container_permissions_interface.h"

namespace swganh
{
namespace object
{

class CreaturePermissions : public ContainerPermissionsInterface
{
	bool canInsert(std::shared_ptr<Object> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object);
	bool canRemove(std::shared_ptr<Object> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object);
	bool canView(std::shared_ptr<Object> container, std::shared_ptr<Object> requester);
};

}
}

#endif