
#include "swganh/object/resource_container/resource_container_factory.h"

#include "swganh/object/resource_container/resource_container.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object::resource_container;

void ResourceContainerFactory::LoadTemplates()
{}

bool ResourceContainerFactory::HasTemplate(const string& template_name)
{
    return false;
}

void ResourceContainerFactory::PersistObject(const shared_ptr<Object>& object)
{}

void ResourceContainerFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{}

shared_ptr<Object> ResourceContainerFactory::CreateObjectFromStorage(uint64_t object_id)
{
    return make_shared<ResourceContainer>();
}

shared_ptr<Object> ResourceContainerFactory::CreateObjectFromTemplate(const string& template_name)
{
    return make_shared<ResourceContainer>();
}
