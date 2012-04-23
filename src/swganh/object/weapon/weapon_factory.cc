// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "swganh/object/weapon/weapon_factory.h"

#include "swganh/object/weapon/weapon.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object::weapon;

void WeaponFactory::LoadTemplates()
{}

bool WeaponFactory::HasTemplate(const string& template_name)
{
    return false;
}

void WeaponFactory::PersistObject(const shared_ptr<Object>& object)
{}

void WeaponFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{}

shared_ptr<Object> WeaponFactory::CreateObjectFromStorage(uint64_t object_id)
{
    return make_shared<Weapon>();
}

shared_ptr<Object> WeaponFactory::CreateObjectFromTemplate(const string& template_name)
{
    return make_shared<Weapon>();
}
