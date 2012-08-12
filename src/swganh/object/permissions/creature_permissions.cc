#include "creature_permissions.h"

using namespace swganh::object;

bool CreaturePermissions::canInsert(std::shared_ptr<Object> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object)
{
	return (container == requester);
}

bool CreaturePermissions::canRemove(std::shared_ptr<Object> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object)
{
	return (container == requester);
}

bool CreaturePermissions::canView(std::shared_ptr<Object> container, std::shared_ptr<Object> requester)
{
	return true;
}