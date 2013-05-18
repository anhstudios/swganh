// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "manufacture_schematic_factory.h"

#include "manufacture_schematic.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object;

ManufactureSchematicFactory::ManufactureSchematicFactory(swganh::app::SwganhKernel* kernel)
	: IntangibleFactory(kernel)
{
}

void ManufactureSchematicFactory::LoadFromStorage(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Object>& object, boost::unique_lock<boost::mutex>& lock)
{
    IntangibleFactory::LoadFromStorage(connection, object, lock);
}

uint32_t ManufactureSchematicFactory::PersistObject(const shared_ptr<Object>& object, boost::unique_lock<boost::mutex>& lock, bool persist_inherited)
{
	return 0;
}

void ManufactureSchematicFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{
	ObjectFactory::DeleteObjectFromStorage(object);
}

shared_ptr<Object> ManufactureSchematicFactory::CreateObject()
{
    return make_shared<ManufactureSchematic>();
}
