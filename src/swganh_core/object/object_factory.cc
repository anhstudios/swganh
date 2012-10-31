// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

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
#include "swganh/simulation/simulation_service_interface.h"
#include "swganh/tre/resource_manager.h"
#include "swganh/tre/visitors/objects/object_visitor.h"

#include "swganh/object/permissions/container_permissions_interface.h"

using namespace sql;
using namespace std;
using namespace swganh::app;
using namespace swganh::database;
using namespace swganh::object;
using namespace swganh::simulation;
using namespace swganh::tre;

ObjectFactory::ObjectFactory(SwganhKernel* kernel)
	: kernel_(kernel)
{
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
		if(object->IsDatabasePersisted())
			PersistObject(object);
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
uint32_t ObjectFactory::PersistObject(const shared_ptr<Object>& object, bool persist_inherited)
{
	uint32_t counter = 1;
    try {
        auto conn = GetDatabaseManager()->getConnection("galaxy");
        auto prepared_statement = shared_ptr<sql::PreparedStatement>
            (conn->prepareStatement("CALL sp_PersistObject(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);"));
        prepared_statement->setUInt64(counter++, object->GetObjectId());
		if (object->GetContainer() != nullptr)
		{
			prepared_statement->setUInt(counter++, object->GetSceneId());
			prepared_statement->setUInt64(counter++, object->GetContainer()->GetObjectId());
		}
        else
		{
			prepared_statement->setUInt(counter++, object->GetSceneId());
            prepared_statement->setUInt64(counter++, 0);
		}
        prepared_statement->setString(counter++, object->GetTemplate());
        auto position = object->GetPosition();
        prepared_statement->setDouble(counter++, position.x);
        prepared_statement->setDouble(counter++, position.y);
        prepared_statement->setDouble(counter++, position.z);
        auto orientation = object->GetOrientation();
        prepared_statement->setDouble(counter++, orientation.x);
        prepared_statement->setDouble(counter++, orientation.y);
        prepared_statement->setDouble(counter++, orientation.z);
        prepared_statement->setDouble(counter++, orientation.w);
        prepared_statement->setDouble(counter++, object->GetComplexity());
        prepared_statement->setString(counter++, object->GetStfNameFile());
        prepared_statement->setString(counter++, object->GetStfNameString());
        auto custom_name = object->GetCustomName();
        prepared_statement->setString(counter++, string(begin(custom_name), end(custom_name)));
        prepared_statement->setUInt(counter++, object->GetVolume());
		prepared_statement->setInt(counter++, object->GetArrangementId());
		prepared_statement->setInt(counter++, object->GetPermissions()->GetType());
		prepared_statement->setInt(counter++, object->GetType());
        // Now execute the update
        prepared_statement->executeUpdate();

		// Now Persist the objects
		PersistAttributes(object);
	}
    catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
	return counter;
}

void ObjectFactory::CreateBaseObjectFromStorage(const shared_ptr<Object>& object, const shared_ptr<sql::ResultSet>& result)
{
    try {
        result->next();
        // Set Event Dispatcher
        object->SetEventDispatcher(GetEventDispatcher());
        object->SetSceneId(result->getUInt("scene_id"));
        object->SetPosition(glm::vec3(result->getDouble("x_position"),result->getDouble("y_position"), result->getDouble("z_position")));
        object->SetOrientation(glm::quat(
            static_cast<float>(result->getDouble("w_orientation")),
			static_cast<float>(result->getDouble("x_orientation")),
            static_cast<float>(result->getDouble("y_orientation")),
            static_cast<float>(result->getDouble("z_orientation"))));

        object->SetComplexity(static_cast<float>(result->getDouble("complexity")));
        object->SetStfName(result->getString("stf_name_file"),
                           result->getString("stf_name_string"));
        string custom_string = result->getString("custom_name");
        object->SetCustomName(wstring(begin(custom_string), end(custom_string)));
        object->SetVolume(result->getUInt("volume"));
        object->SetTemplate(result->getString("iff_template"));
		object->SetArrangementId(result->getInt("arrangement_id"));

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
		object_manager_->LoadSlotsForObject(object);
		object_manager_->LoadCollisionInfoForObject(object);

		auto parent = object_manager_->GetObjectById(result->getUInt64("parent_id"));
		if(parent != nullptr)
		{
			parent->AddObject(nullptr, object);
		}
		// Attribute Template ID
		int attribute_template_id = result->getInt("attribute_template_id");
		object->SetAttributeTemplateId(attribute_template_id);

		LoadAttributes(object);

		GetClientData(object);

		//Clear us from the db persist update queue.
		boost::lock_guard<boost::mutex> lock(persisted_objects_mutex_);
		auto find_itr = persisted_objects_.find(object);
		if(find_itr != persisted_objects_.end())
			persisted_objects_.erase(find_itr);

    }
    catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

void ObjectFactory::LoadAttributes(std::shared_ptr<Object> object)
{
	 try {
        auto conn = GetDatabaseManager()->getConnection("galaxy");
        auto statement = conn->prepareStatement("CALL sp_GetAttributes(?,?);");
		statement->setString(1, object->GetTemplate());
        statement->setUInt64(2, object->GetObjectId());
        auto result = unique_ptr<sql::ResultSet>(statement->executeQuery());        
        while (result->next())
        {
			string attr_name = result->getString("name");
			string unparsed_value = result->getString("attribute_value");
			try {				
				if (std::string::npos != unparsed_value.find("."))
				{
					object->SetAttribute(attr_name, boost::lexical_cast<float>(unparsed_value));
				}
				else if (unparsed_value.find_first_of("0123456789") == 0)
				{
					object->SetAttribute(attr_name, boost::lexical_cast<int>(unparsed_value));
				}
				else
				{
					object->SetAttribute(attr_name, std::wstring(unparsed_value.begin(), unparsed_value.end()));
				}
			}
			catch (std::exception& e)
			{
				LOG(error) << "Error parsing attribute " << attr_name <<" for object_id:" << object->GetObjectId() << " error message:" << e.what();
				object->SetAttribute(attr_name, std::wstring(unparsed_value.begin(), unparsed_value.end()));
			}
        }          
    }
    catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();        
    }
}
void ObjectFactory::PersistAttributes(std::shared_ptr<Object> object)
{
	try 
	{
		auto conn = GetDatabaseManager()->getConnection("galaxy");
		for (auto& attribute : object->GetAttributeMap())
		{
			auto statement = conn->prepareStatement("CALL sp_PersistAttribute(?,?,?);");
			statement->setUInt64(1, object->GetObjectId());
			std::string name = attribute.first.ident_string();
			statement->setString(2, name);
			std::wstring attr = object->GetAttributeRecursiveAsString(name);
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
		LOG(error) << "error persisting attributes for object " << object->GetObjectId() + " " << ex.what();
	}
}

uint32_t ObjectFactory::LookupType(uint64_t object_id)
{
    uint32_t type = 0;
    try {
		auto conn = GetDatabaseManager()->getConnection("galaxy");
        auto statement = conn->prepareStatement("CALL sp_GetType(?);");
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

        uint64_t contained_id;
        uint32_t contained_type;

        while (result->next())
        {
            contained_id = result->getUInt64("id");
            contained_type = result->getUInt("type_id");

            auto contained_object = object_manager_->CreateObjectFromStorage(contained_id, contained_type);

			if(contained_object->GetArrangementId() == -2)
			{
				//This object has never been loaded before and needs to be put into the default slot.
				object->AddObject(nullptr, contained_object);
			}
			else 
			{
				//Put it back where it was persisted
				object->AddObject(nullptr, contained_object, contained_object->GetArrangementId());
			}

        }
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
void ObjectFactory::GetClientData(const std::shared_ptr<Object>& object)
{
	try {

		auto oiff = kernel_->GetResourceManager()->GetResourceByName<ObjectVisitor>(object->GetTemplate());
		auto object_name = oiff->attribute<shared_ptr<ObjectVisitor::ClientString>>("objectName");
		if (object->GetStfNameFile().length() == 0)
		{
			object->SetStfName(object_name->file, object_name->entry);
		}

	} catch (std::exception& /*ex*/) {
		//LOG(warning) << "Client data not found for object: " << object->GetObjectId() << " with error:" << ex.what();
	}
}