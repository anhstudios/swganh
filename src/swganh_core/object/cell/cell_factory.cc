// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "cell_factory.h"

#include "cell.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object;

uint32_t CellFactory::PersistObject(const shared_ptr<Object>& object)
{
	return 0;
}

void CellFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{}

shared_ptr<Object> CellFactory::CreateObjectFromStorage(uint64_t object_id)
{
	//TODO: Use the db to fetch me
    return make_shared<Cell>();
}

shared_ptr<Object> CellFactory::CreateObjectFromTemplate(const string& template_name, bool db_persisted, bool db_initialized)
{
	if(db_persisted || db_initialized)
	{
		//TODO: Have to hit the db to make this
		return make_shared<Cell>();
	}
	else
	{
		return make_shared<Cell>();
	}
}
