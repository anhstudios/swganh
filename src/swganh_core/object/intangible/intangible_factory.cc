// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "intangible_factory.h"

#include <sstream>

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#include "swganh/logger.h"

#include "swganh/database/database_manager.h"
#include "swganh_core/object/intangible/intangible.h"
#include "swganh_core/object/exception.h"
#include "swganh/simulation/simulation_service_interface.h"

using namespace std;
using namespace swganh::database;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::simulation;

IntangibleFactory::IntangibleFactory(DatabaseManagerInterface* db_manager,
                             swganh::EventDispatcher* event_dispatcher)
    : ObjectFactory(db_manager, event_dispatcher)
{
}

uint32_t IntangibleFactory::PersistObject(const shared_ptr<Object>& object)
{
	return 0;
}

void IntangibleFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{}

shared_ptr<Object> IntangibleFactory::CreateObjectFromStorage(uint64_t object_id)
{
    //@TODO: Load me from storage
    return make_shared<Intangible>();
}

shared_ptr<Object> IntangibleFactory::CreateObjectFromTemplate(const string& template_name, bool db_persisted, bool db_initialized)
{
	//@TODO: Create me with help from db
    return make_shared<Intangible>();
}
