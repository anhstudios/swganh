#include "creature_permissions.h"

#include "swganh/object/object.h"
#include "swganh/object/container_interface.h"

using namespace swganh::object;

bool CreaturePermissions::canInsert(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object)
{
	return (container->GetObjectId() == requester->GetObjectId());
}

bool CreaturePermissions::canRemove(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object)
{
	return (container->GetObjectId() == requester->GetObjectId());
}

bool CreaturePermissions::canView(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester)
{
	return true;
}