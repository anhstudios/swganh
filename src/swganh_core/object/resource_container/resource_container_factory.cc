// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "resource_container_factory.h"
#include "resource_container.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object;

ResourceContainerFactory::ResourceContainerFactory(swganh::database::DatabaseManagerInterface* db_manager,
            swganh::EventDispatcher* event_dispatcher)
	: TangibleFactory(db_manager, event_dispatcher)
{
}

uint32_t ResourceContainerFactory::PersistObject(const shared_ptr<Object>& object)
{
	uint32_t counter = 1;
	return counter;
}

void ResourceContainerFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{}

shared_ptr<Object> ResourceContainerFactory::CreateObjectFromStorage(uint64_t object_id)
{
    return make_shared<ResourceContainer>();
}

shared_ptr<Object> ResourceContainerFactory::CreateObjectFromTemplate(const string& template_name, bool db_persisted, bool db_initialized)
{
	//@TODO: Create me with help from db
    return make_shared<ResourceContainer>();
}
