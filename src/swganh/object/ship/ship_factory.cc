
#include "swganh/object/ship/ship_factory.h"

#include "swganh/object/ship/ship.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object::ship;

void ShipFactory::LoadTemplates()
{}

bool ShipFactory::HasTemplate(const string& template_name)
{
    return false;
}

void ShipFactory::PersistObject(const shared_ptr<Object>& object)
{}

void ShipFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{}

shared_ptr<Object> ShipFactory::CreateObjectFromStorage(uint64_t object_id)
{
    return make_shared<Ship>();
}

shared_ptr<Object> ShipFactory::CreateObjectFromTemplate(const string& template_name)
{
    return make_shared<Ship>();
}
