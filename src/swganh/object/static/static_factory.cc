
#include "swganh/object/static/static_factory.h"

#include "swganh/object/static/static.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object::static_obj;

void StaticFactory::LoadTemplates()
{}

bool StaticFactory::HasTemplate(const string& template_name)
{
    return false;
}

void StaticFactory::PersistObject(const shared_ptr<Object>& object)
{}

void StaticFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{}

shared_ptr<Object> StaticFactory::CreateObjectFromStorage(uint64_t object_id)
{
    return make_shared<Static>();
}

shared_ptr<Object> StaticFactory::CreateObjectFromTemplate(const string& template_name)
{
    return make_shared<Static>();
}
