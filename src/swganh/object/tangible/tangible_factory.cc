
#include "swganh/object/tangible/tangible_factory.h"

#include "swganh/object/tangible/tangible.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object::tangible;

void TangibleFactory::LoadTemplates()
{}

bool TangibleFactory::HasTemplate(const string& template_name)
{
    return false;
}

void TangibleFactory::PersistObject(const shared_ptr<Object>& object)
{}

void TangibleFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{}

shared_ptr<Object> TangibleFactory::CreateObjectFromStorage(uint64_t object_id)
{
    return make_shared<Tangible>();
}

shared_ptr<Object> TangibleFactory::CreateObjectFromTemplate(const string& template_name)
{
    return make_shared<Tangible>();
}
