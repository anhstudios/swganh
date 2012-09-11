// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "manufacture_schematic_factory.h"

#include "manufacture_schematic.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object;

ManufactureSchematicFactory::ManufactureSchematicFactory(swganh::database::DatabaseManagerInterface* db_manager, swganh::EventDispatcher* event_dispatcher)
	: IntangibleFactory(db_manager, event_dispatcher)
{
}

uint32_t ManufactureSchematicFactory::PersistObject(const shared_ptr<Object>& object)
{
	return 0;
}

void ManufactureSchematicFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{}

shared_ptr<Object> ManufactureSchematicFactory::CreateObjectFromStorage(uint64_t object_id)
{
    return make_shared<ManufactureSchematic>();
}

shared_ptr<Object> ManufactureSchematicFactory::CreateObjectFromTemplate(const string& template_name, bool db_persisted, bool db_initialized)
{
	//@TODO: Create me with help from db
    return make_shared<ManufactureSchematic>();
}
