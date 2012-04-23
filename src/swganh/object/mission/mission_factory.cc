// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "swganh/object/mission/mission_factory.h"

#include "swganh/object/mission/mission.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object::mission;

void MissionFactory::LoadTemplates()
{}

bool MissionFactory::HasTemplate(const string& template_name)
{
    return false;
}

void MissionFactory::PersistObject(const shared_ptr<Object>& object)
{}

void MissionFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{}

shared_ptr<Object> MissionFactory::CreateObjectFromStorage(uint64_t object_id)
{
    return make_shared<Mission>();
}

shared_ptr<Object> MissionFactory::CreateObjectFromTemplate(const string& template_name)
{
    return make_shared<Mission>();
}
