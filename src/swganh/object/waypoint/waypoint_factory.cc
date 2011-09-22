
#include "swganh/object/waypoint/waypoint_factory.h"

#include "swganh/object/waypoint/waypoint.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object::waypoint;

void WaypointFactory::LoadTemplates()
{}

bool WaypointFactory::HasTemplate(const string& template_name)
{
    return false;
}

void WaypointFactory::PersistObject(const shared_ptr<Object>& object)
{}

void WaypointFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{}

shared_ptr<Object> WaypointFactory::CreateObjectFromStorage(uint64_t object_id)
{
    return make_shared<Waypoint>();
}

shared_ptr<Object> WaypointFactory::CreateObjectFromTemplate(const string& template_name)
{
    return make_shared<Waypoint>();
}
