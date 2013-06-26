// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef WIN32
#include <Python.h>
#endif

#include "object_factory.h"


#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#include "swganh/logger.h"

#include "swganh/database/database_manager.h"
#include "swganh_core/object/object.h"
#include "swganh_core/object/object_manager.h"
#include "swganh_core/object/exception.h"
#include "swganh_core/simulation/simulation_service_interface.h"
#include "swganh/tre/resource_manager.h"
#include "swganh/tre/visitors/objects/object_visitor.h"

#include "swganh_core/object/permissions/container_permissions_interface.h"

using namespace sql;
using namespace std;
using namespace swganh::app;
using namespace swganh::database;
using namespace swganh::object;
using namespace swganh::simulation;
using namespace swganh::tre;

ObjectFactory::ObjectFactory(SwganhKernel* kernel)
	: kernel_(kernel)
{}

std::future<std::shared_ptr<Object>> ObjectFactory::LoadFromStorage(uint64_t object_id)
{
    return GetDatabaseManager()->ExecuteAsync(std::bind(&ObjectFactory::LoadDataFromStorage, this, std::placeholders::_1, object_id), "galaxy");
}

std::shared_ptr<Object> ObjectFactory::LoadDataFromStorage(const std::shared_ptr<sql::Connection>& connection, uint64_t object_id)
{
    auto object = CreateObject();
    object->SetEventDispatcher(GetEventDispatcher());
    
	auto lock = object->AcquireLock();
	object->SetObjectId(object_id, lock);

    LoadFromStorage(connection, object, lock);

    return object;
}

void ObjectFactory::LoadFromStorage(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Object>& object, boost::unique_lock<boost::mutex>& lock)
{
    std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement("CALL sp_GetObject(?);"));

    statement->setUInt64(1, object->GetObjectId(lock));
    
    std::unique_ptr<sql::ResultSet> result(statement->executeQuery());        
    
    do
    {
        while (result->next())
        {
            object->SetEventDispatcher(GetEventDispatcher());
            object->SetSceneId(result->getUInt("scene_id"), lock);
            object->SetPosition(glm::vec3(result->getDouble("x_position"),result->getDouble("y_position"), result->getDouble("z_position")), lock);

            object->SetOrientation(glm::quat(
                static_cast<float>(result->getDouble("w_orientation")),
            	static_cast<float>(result->getDouble("x_orientation")),
                static_cast<float>(result->getDouble("y_orientation")),
                static_cast<float>(result->getDouble("z_orientation"))), lock);
            
            object->SetComplexity(static_cast<float>(result->getDouble("complexity")), lock);
            object->SetStfName(result->getString("stf_name_file"), 
								result->getString("stf_name_string"), lock);

            string custom_string = result->getString("custom_name");
            object->SetCustomName(wstring(begin(custom_string), end(custom_string)), lock);
            object->SetVolume(result->getUInt("volume"), lock);
            object->SetTemplate(result->getString("iff_template"), lock);
            object->SetArrangementId(result->getInt("arrangement_id"));
            object->SetAttributeTemplateId(result->getInt("attribute_template_id"), lock);
            auto permissions_objects_ = object_manager_->GetPermissionsMap();
            auto permissions_itr = permissions_objects_.find(result->getInt("permission_type"));
            if(permissions_itr != permissions_objects_.end())
            {
            	object->SetPermissions(permissions_itr->second);
            }
            else
            {
            	DLOG(error) << "FAILED TO FIND PERMISSION TYPE " << result->getInt("perission_type");
            	object->SetPermissions(permissions_objects_.find(DEFAULT_PERMISSION)->second);
            }

            auto parent = object_manager_->GetObjectById(result->getUInt64("parent_id"));
            if(parent != nullptr)
            {
            	parent->AddObject(nullptr, object);
            }
        }
    } while(statement->getMoreResults());

	LoadAttributes(connection, object, lock);
	GetClientData(object, lock);
}

void ObjectFactory::RegisterEventHandlers()
{
	GetEventDispatcher()->Subscribe("Object::CustomName", std::bind(&ObjectFactory::PersistHandler, this, std::placeholders::_1));
    GetEventDispatcher()->Subscribe("Object::StfName", std::bind(&ObjectFactory::PersistHandler, this, std::placeholders::_1));
    GetEventDispatcher()->Subscribe("Object::Complexity", std::bind(&ObjectFactory::PersistHandler, this, std::placeholders::_1));
    GetEventDispatcher()->Subscribe("Object::Volume", std::bind(&ObjectFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Object::Template", std::bind(&ObjectFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Object::Position", std::bind(&ObjectFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Object::Orientation", std::bind(&ObjectFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Object::Container", std::bind(&ObjectFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Object::StfName", std::bind(&ObjectFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Object::SceneId", std::bind(&ObjectFactory::PersistHandler, this, std::placeholders::_1));
}

void ObjectFactory::PersistChangedObjects()
{
	std::set<shared_ptr<Object>> persisted;
	{
		boost::lock_guard<boost::mutex> lg(persisted_objects_mutex_);
		persisted = move(persisted_objects_);
	}
	for (auto& object : persisted)
	{
		auto lock = object->AcquireLock();
		if(object->IsDatabasePersisted(lock))
		{
			PersistObject(object, lock);
		}
	}
}

void ObjectFactory::PersistHandler(const shared_ptr<swganh::EventInterface>& incoming_event)
{
	auto object = static_pointer_cast<ObjectEvent>(incoming_event)->Get();
	if (object && object->IsDatabasePersisted())
	{
		boost::lock_guard<boost::mutex> lg(persisted_objects_mutex_);
		persisted_objects_.insert(object);
	}
}

uint32_t ObjectFactory::PersistObject(const shared_ptr<Object>& object, boost::unique_lock<boost::mutex>& lock, bool persist_inherited)
{
	uint32_t counter = 1;
    try {
        auto conn = GetDatabaseManager()->getConnection("galaxy");
        auto prepared_statement = shared_ptr<sql::PreparedStatement>
            (conn->prepareStatement("CALL sp_PersistObject(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);"));
        prepared_statement->setUInt64(counter++, object->GetObjectId(lock));
		auto container = object->GetContainer(lock);
		if (container != nullptr)
		{
			prepared_statement->setUInt(counter++, object->GetSceneId(lock));

			//We need to make sure we only have one object locked a time.
			lock.unlock();
			prepared_statement->setUInt64(counter++, container->GetObjectId());
			lock.lock();
		}
        else
		{
			prepared_statement->setUInt(counter++, object->GetSceneId(lock));
            prepared_statement->setUInt64(counter++, 0);
		}
        prepared_statement->setString(counter++, object->GetTemplate(lock));
        auto position = object->GetPosition(lock);
        prepared_statement->setDouble(counter++, position.x);
        prepared_statement->setDouble(counter++, position.y);
        prepared_statement->setDouble(counter++, position.z);
        auto orientation = object->GetOrientation(lock);
        prepared_statement->setDouble(counter++, orientation.x);
        prepared_statement->setDouble(counter++, orientation.y);
        prepared_statement->setDouble(counter++, orientation.z);
        prepared_statement->setDouble(counter++, orientation.w);
        prepared_statement->setDouble(counter++, object->GetComplexity(lock));
        prepared_statement->setString(counter++, object->GetStfNameFile(lock));
        prepared_statement->setString(counter++, object->GetStfNameString(lock));
        auto custom_name = object->GetCustomName(lock);
        prepared_statement->setString(counter++, string(begin(custom_name), end(custom_name)));
        prepared_statement->setUInt(counter++, object->GetVolume(lock));
		prepared_statement->setInt(counter++, object->GetArrangementId());
		prepared_statement->setInt(counter++, object->GetPermissions()->GetType());
		prepared_statement->setInt(counter++, object->GetType());
        // Now execute the update
        prepared_statement->executeUpdate();

		// Now Persist the objects
		PersistAttributes(object, lock);
	}
    catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
	return counter;
}

void ObjectFactory::LoadAttributes(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Object>& object,
								   boost::unique_lock<boost::mutex>& lock)
{
    auto statement = connection->prepareStatement("CALL sp_GetAttributes(?);");

    statement->setUInt64(1, object->GetObjectId(lock));

    auto result = unique_ptr<sql::ResultSet>(statement->executeQuery());
    do
    {
        while (result->next())
        {
            string attr_name = result->getString("name");
            string unparsed_value = result->getString("attribute_value");

            if(auto int_value = IsInteger(unparsed_value))
            {
                object->SetAttribute(attr_name, *int_value, lock);
            }
            else if(auto float_value = IsFloat(unparsed_value))
            {
                object->SetAttribute(attr_name, *float_value, lock);
            }
            else
            {
                object->SetAttribute(attr_name, std::wstring(unparsed_value.begin(), unparsed_value.end()), lock);
            }
        }
    } while(statement->getMoreResults());
}

void ObjectFactory::PersistAttributes(std::shared_ptr<Object> object, boost::unique_lock<boost::mutex>& lock)
{
	try
	{
		auto conn = GetDatabaseManager()->getConnection("galaxy");
		for (auto& attribute : object->GetAttributeMap(lock))
		{
			auto statement = conn->prepareStatement("CALL sp_PersistAttribute(?,?,?);");
			statement->setUInt64(1, object->GetObjectId(lock));
			std::string name = attribute.first.ident_string();
			statement->setString(2, name);
			std::wstring attr = object->GetAttributeAsString(name, lock);
			statement->setString(3, std::string(attr.begin(), attr.end()));

			statement->executeUpdate();
		}
	}
	catch(sql::SQLException &e)
	{
		LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
		LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
	}
	catch (std::exception& ex)
	{
		LOG(error) << "error persisting attributes for object " << object->GetObjectId() << " " << ex.what();
	}
}

uint32_t ObjectFactory::LookupType(uint64_t object_id)
{
    uint32_t type = 0;
    try {
		auto conn = GetDatabaseManager()->getConnection("galaxy");
        auto statement = conn->prepareStatement("CALL sp_GetObjectType(?);");
        statement->setUInt64(1, object_id);
        auto result = unique_ptr<sql::ResultSet>(statement->executeQuery());
        while (result->next())
        {
            type = result->getUInt("description");
        }
        return type;
    }
    catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
        return type;
    }
}

void ObjectFactory::LoadContainedObjects(
    const shared_ptr<Object>& object,
    const shared_ptr<Statement>& statement)
{
    // Check for contained objects
    if (statement->getMoreResults())
    {
        unique_ptr<ResultSet> result(statement->getResultSet());
        LoadContainedObjects(object, result);
    }
}

void ObjectFactory::LoadContainedObjects(
    const shared_ptr<Object>& object,
    const unique_ptr<ResultSet>& result)
{
    while (result->next())
    {
        uint64_t contained_id = result->getUInt64("id");
        uint32_t contained_type = result->getUInt("type_id");

        auto contained_object = object_manager_->CreateObjectFromStorage(contained_id, contained_type);

        object->AddObject(nullptr, contained_object, contained_object->GetArrangementId());
    }
}

void ObjectFactory::LoadContainedObjects(const std::shared_ptr<Object>& object)
{
    try
    {
        auto conn = GetDatabaseManager()->getConnection("galaxy");
        auto statement = shared_ptr<sql::PreparedStatement>(
            conn->prepareStatement("CALL sp_GetContainedObjects(?);"));

        statement->setUInt64(1, object->GetObjectId());

        LoadContainedObjects(object, unique_ptr<ResultSet>(statement->executeQuery()));
    }
    catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

void ObjectFactory::DeleteObjectFromStorage(const std::shared_ptr<Object>& object)
{
	try {
        auto conn = GetDatabaseManager()->getConnection("galaxy");
        auto statement = conn->prepareStatement("CALL sp_DeleteObject(?,?);");
        statement->setUInt64(1, object->GetObjectId());
		statement->setInt(2, object->GetType());
        statement->execute();
    }
    catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

void ObjectFactory::GetClientData(const std::shared_ptr<Object>& object,boost::unique_lock<boost::mutex>& lock)
{
	try {

		auto oiff = kernel_->GetResourceManager()->GetResourceByName<ObjectVisitor>(object->GetTemplate(lock));
		auto object_name = oiff->attribute<shared_ptr<ObjectVisitor::ClientString>>("objectName");
		if (object->GetStfNameFile(lock).length() == 0)
		{
			object->SetStfName(object_name->file, object_name->entry, lock);
		}

	} catch (std::exception& /*ex*/) {
		//LOG(warning) << "Client data not found for object: " << object->GetObjectId() << " with error:" << ex.what();
	}
}

boost::optional<float> ObjectFactory::IsFloat(const std::string& value) const
{
    boost::optional<float> float_value;
    try {
        float_value = boost::lexical_cast<float>(value);
    } catch(std::exception&) {}

    return float_value;
}

boost::optional<int64_t> ObjectFactory::IsInteger(const std::string& value) const
{
    boost::optional<int64_t> int_value;
    try {
        int_value = boost::lexical_cast<int64_t>(value);
    } catch(std::exception&) {}

    return int_value;
}
