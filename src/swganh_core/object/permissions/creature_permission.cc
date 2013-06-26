// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "creature_permission.h"

#include "swganh_core/object/object.h"
#include "swganh_core/object/container_interface.h"

using namespace swganh::object;

bool CreaturePermission::canInsert(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object)
{
	return (container->GetObjectId() == requester->GetObjectId());
}

bool CreaturePermission::canRemove(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object)
{
	return (container->GetObjectId() == requester->GetObjectId());
}

bool CreaturePermission::canView(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester)
{
	return true;
}