// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "swganh/object/manufacture_schematic/manufacture_schematic_factory.h"

#include "swganh/object/manufacture_schematic/manufacture_schematic.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object::manufacture_schematic;

void ManufactureSchematicFactory::LoadTemplates()
{}

bool ManufactureSchematicFactory::HasTemplate(const string& template_name)
{
    return false;
}

void ManufactureSchematicFactory::PersistObject(const shared_ptr<Object>& object)
{}

void ManufactureSchematicFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{}

shared_ptr<Object> ManufactureSchematicFactory::CreateObjectFromStorage(uint64_t object_id)
{
    return make_shared<ManufactureSchematic>();
}

shared_ptr<Object> ManufactureSchematicFactory::CreateObjectFromTemplate(const string& template_name)
{
    return make_shared<ManufactureSchematic>();
}
