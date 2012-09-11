// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "installation_factory.h"

#include "installation.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object;

uint32_t InstallationFactory::PersistObject(const shared_ptr<Object>& object)
{
	uint32_t counter = 1;

	return counter;
}

void InstallationFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{}

shared_ptr<Object> InstallationFactory::CreateObjectFromStorage(uint64_t object_id)
{
    return make_shared<Installation>();
}

shared_ptr<Object> InstallationFactory::CreateObjectFromTemplate(const string& template_name, bool db_persisted, bool db_initialized)
{
	//@TODO: Create me with help from db
    return make_shared<Installation>();
}
