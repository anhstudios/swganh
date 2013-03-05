// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "ridable_permission.h"

#include "swganh_core/object/container_interface.h"
#include "swganh_core/object/creature/creature.h"

using namespace swganh::object;

bool RideablePermission::canInsert(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object)
{
	std::shared_ptr<Creature> creo = std::static_pointer_cast<Creature>(container);
	return requester == object && creo && creo->GetOwnerId() == requester->GetObjectId();
}

bool RideablePermission::canRemove(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object)
{
	std::shared_ptr<Creature> creo = std::static_pointer_cast<Creature>(container);
	return requester == object && creo && creo->GetOwnerId() == requester->GetObjectId();
}

bool RideablePermission::canView(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester)
{
	return true;
}