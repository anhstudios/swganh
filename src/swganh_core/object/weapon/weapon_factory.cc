// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "weapon_factory.h"

#include "swganh_core/object/weapon/weapon.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object;

WeaponFactory::WeaponFactory(swganh::database::DatabaseManagerInterface* db_manager, swganh::EventDispatcher* event_dispatcher)
	: TangibleFactory(db_manager, event_dispatcher)
{
}

uint32_t WeaponFactory::PersistObject(const shared_ptr<Object>& object)
{
	return 1;
}

void WeaponFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{}

shared_ptr<Object> WeaponFactory::CreateObjectFromStorage(uint64_t object_id)
{
    return make_shared<Weapon>();
}

shared_ptr<Object> WeaponFactory::CreateObjectFromTemplate(const string& template_name, bool db_persisted, bool db_initialized)
{
	//@TODO: Create me with help from db
    return make_shared<Weapon>();
}
