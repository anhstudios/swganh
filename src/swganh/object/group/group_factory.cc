// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "swganh/object/group/group_factory.h"

#include "swganh/object/group/group.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object::group;

void GroupFactory::LoadTemplates()
{}

bool GroupFactory::HasTemplate(const string& template_name)
{
    return false;
}

void GroupFactory::PersistObject(const shared_ptr<Object>& object)
{}

void GroupFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{}

shared_ptr<Object> GroupFactory::CreateObjectFromStorage(uint64_t object_id)
{
    return make_shared<Group>();
}

shared_ptr<Object> GroupFactory::CreateObjectFromTemplate(const string& template_name)
{
    return make_shared<Group>();
}
