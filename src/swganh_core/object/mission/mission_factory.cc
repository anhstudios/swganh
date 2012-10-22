// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "mission_factory.h"

#include "mission.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object;

MissionFactory::MissionFactory(swganh::app::SwganhKernel* kernel)
	: IntangibleFactory(kernel)
{
}

uint32_t MissionFactory::PersistObject(const shared_ptr<Object>& object)
{
	uint32_t counter = 1;

	return counter;
}

void MissionFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{
	ObjectFactory::DeleteObjectFromStorage(object);
}

shared_ptr<Object> MissionFactory::CreateObjectFromStorage(uint64_t object_id)
{
    return make_shared<Mission>();
}

shared_ptr<Object> MissionFactory::CreateObject()
{
    return make_shared<Mission>();
}
