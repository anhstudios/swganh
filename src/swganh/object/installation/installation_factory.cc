
#include "swganh/object/installation/installation_factory.h"

#include "swganh/object/installation/installation.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object::installation;

void InstallationFactory::LoadTemplates()
{}

bool InstallationFactory::HasTemplate(const string& template_name)
{
    return false;
}

void InstallationFactory::PersistObject(const shared_ptr<Object>& object)
{}

void InstallationFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{}

shared_ptr<Object> InstallationFactory::CreateObjectFromStorage(uint64_t object_id)
{
    return make_shared<Installation>();
}

shared_ptr<Object> InstallationFactory::CreateObjectFromTemplate(const string& template_name)
{
    return make_shared<Installation>();
}
