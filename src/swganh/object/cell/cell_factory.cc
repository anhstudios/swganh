// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "swganh/object/cell/cell_factory.h"

#include "swganh/object/cell/cell.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object::cell;

void CellFactory::LoadTemplates()
{}

bool CellFactory::HasTemplate(const string& template_name)
{
    return false;
}

void CellFactory::PersistObject(const shared_ptr<Object>& object)
{}

void CellFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{}

shared_ptr<Object> CellFactory::CreateObjectFromStorage(uint64_t object_id)
{
    return make_shared<Cell>();
}

shared_ptr<Object> CellFactory::CreateObjectFromTemplate(const string& template_name)
{
    return make_shared<Cell>();
}
