#include "ridable_permissions.h"

using namespace swganh::object;

bool RidablePermissions::canInsert(std::shared_ptr<Object> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object)
{
	return false;
}

bool RidablePermissions::canRemove(std::shared_ptr<Object> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object)
{
	return false;
}

bool RidablePermissions::canView(std::shared_ptr<Object> container, std::shared_ptr<Object> requester)
{
	return false;
}