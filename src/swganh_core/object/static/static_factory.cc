// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "static_factory.h"
#include "static.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object;

StaticFactory::StaticFactory(swganh::app::SwganhKernel* kernel)
	: ObjectFactory(kernel)
{}

void StaticFactory::LoadFromStorage(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Object>& object, boost::unique_lock<boost::mutex>& lock)
{
    ObjectFactory::LoadFromStorage(connection, object, lock);
}

uint32_t StaticFactory::PersistObject(const shared_ptr<Object>& object, boost::unique_lock<boost::mutex>& lock, bool persist_inherited)
{
	uint32_t counter = 1;

	return counter;
}

void StaticFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{
	ObjectFactory::DeleteObjectFromStorage(object);
}

shared_ptr<Object> StaticFactory::CreateObject()
{
    return make_shared<Static>();
}
