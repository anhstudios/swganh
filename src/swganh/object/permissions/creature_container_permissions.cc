#include "creature_container_permissions.h"

#include "swganh/object/object.h"
#include "swganh/object/container_interface.h"

using namespace swganh::object;

bool CreatureContainerPermissions::canInsert(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object)
{
	return (container->GetContainer() == requester);
}

bool CreatureContainerPermissions::canRemove(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object)
{
	return (container->GetContainer() == requester);
}

bool CreatureContainerPermissions::canView(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester)
{
	return (container->GetContainer() == requester);
}