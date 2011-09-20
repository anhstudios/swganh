
#include "swganh/object/intangible/intangible_factory.h"

#include "swganh/object/intangible/intangible.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object::intangible;

void IntangibleFactory::LoadTemplates()
{}

bool IntangibleFactory::HasTemplate(const string& template_name)
{
    return false;
}

void IntangibleFactory::PersistObject(const shared_ptr<Object>& object)
{}

void IntangibleFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{}

shared_ptr<Object> IntangibleFactory::CreateObjectFromStorage(uint64_t object_id)
{
    return make_shared<Intangible>();
}

shared_ptr<Object> IntangibleFactory::CreateObjectFromTemplate(const string& template_name)
{
    return make_shared<Intangible>();
}
