// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "swganh/simulation/simulation_service.h"

#include <boost/algorithm/string.hpp>

#include "anh/byte_buffer.h"
#include "anh/crc.h"
#include "anh/service/service_manager.h"
#include "anh/database/database_manager.h"
#include "anh/network/soe/server_interface.h"
#include "anh/plugin/plugin_manager.h"

#include "swganh/app/swganh_kernel.h"

#include "swganh/command/command_interface.h"
#include "swganh/command/command_service.h"
#include "swganh/command/python_command_creator.h"

#include "swganh/connection/connection_client.h"
#include "swganh/connection/connection_service.h"

#include "swganh/messages/select_character.h"

#include "swganh/object/object.h"
#include "swganh/object/object_controller.h"
#include "swganh/object/object_manager.h"

// factories
#include "swganh/object/creature/creature_factory.h"
#include "swganh/object/creature/creature.h"
#include "swganh/object/tangible/tangible_factory.h"
#include "swganh/object/tangible/tangible.h"
#include "swganh/object/intangible/intangible_factory.h"
#include "swganh/object/intangible/intangible.h"
#include "swganh/object/player/player_factory.h"
#include "swganh/object/player/player.h"

// message builders
#include "swganh/object/creature/creature_message_builder.h"
#include "swganh/object/tangible/tangible_message_builder.h"
#include "swganh/object/player/player_message_builder.h"

#include "swganh/simulation/scene_manager.h"
#include "swganh/simulation/spatial_provider_interface.h"
#include "swganh/messages/cmd_start_scene.h"
#include "swganh/messages/cmd_scene_ready.h"
#include "swganh/messages/obj_controller_message.h"
#include "swganh/messages/update_containment_message.h"

#include "swganh/simulation/movement_manager.h"

using namespace anh;
using namespace std;
using namespace swganh::connection;
using namespace swganh::messages;
using namespace swganh::network;
using namespace swganh::object;
using namespace swganh::simulation;

using anh::network::soe::ServerInterface;
using anh::network::soe::Session;
using anh::service::ServiceDescription;
using swganh::app::SwganhKernel;

namespace swganh {
namespace simulation {

class SimulationServiceImpl {
public:
    SimulationServiceImpl(SwganhKernel* kernel)
        : kernel_(kernel)
    {
		spatial_provider_ = kernel->GetPluginManager()->CreateObject<SpatialProviderInterface>("SimulationService::SpatialProvider");
    }

    const shared_ptr<ObjectManager>& GetObjectManager()
    {
        if (!object_manager_)
        {
            object_manager_ = make_shared<ObjectManager>(kernel_->GetEventDispatcher());
        }

        return object_manager_;
    }

    const shared_ptr<SceneManager>& GetSceneManager()
    {
        if (!scene_manager_)
        {
            scene_manager_ = make_shared<SceneManager>();
        }

        return scene_manager_;
    }

    MovementManager* GetMovementManager()
    {
        if (!movement_manager_)
        {
			movement_manager_ = make_shared<MovementManager>(kernel_->GetEventDispatcher(), spatial_provider_);
        }

        return movement_manager_.get();
    }

    void PersistObject(uint64_t object_id)
    {
        auto find_iter = loaded_objects_.find(object_id);

        if (find_iter == loaded_objects_.end())
        {
            LOG(warning) << "Nothing to persist, no object saved";
            return;
            //throw swganh::object::InvalidObject("Requested object already loaded");
        }
        object_manager_->PersistObject(find_iter->second);
    }
	void PersistRelatedObjects(uint64_t parent_object_id)
	{
		auto find_iter = loaded_objects_.find(parent_object_id);

        if (find_iter == loaded_objects_.end())
        {
            LOG(warning) << "Nothing to persist, no object saved";
            return;
            //throw swganh::object::InvalidObject("Requested object already loaded");
        }
		// first persist the parent object
		PersistObject(parent_object_id);
		// get all the contained objects
		auto contained_objects = find_iter->second->GetContainedObjects();
		for_each(begin(contained_objects), end(contained_objects), [=](pair<uint64_t, shared_ptr<Object>> pair){
			// if there's objects contained within this object do a recursion call
			auto inner_contained = pair.second->GetContainedObjects();
			if (inner_contained.size() > 0)
			{
				LOG(warning) << "Persist inner container recursively:" << pair.first;
				PersistRelatedObjects(pair.first);
			}
			else
			{
				LOG(warning) << "Persist inner container:" << pair.first;
				PersistObject(pair.first);
			}
		});
	}
    shared_ptr<Object> LoadObjectById(uint64_t object_id)
    {
        auto find_iter = loaded_objects_.find(object_id);

        if (find_iter != loaded_objects_.end())
        {
            return find_iter->second;
            LOG(warning) << "Requested object already loaded";
        }

        auto object = object_manager_->CreateObjectFromStorage(object_id);

        loaded_objects_.insert(make_pair(object_id, object));
		spatial_provider_->AddObject(object); // Add object to spatial indexing.
        return object;
    }
    shared_ptr<Object> LoadObjectById(uint64_t object_id, uint32_t type)
    {
        auto find_iter = loaded_objects_.find(object_id);

        if (find_iter != loaded_objects_.end())
        {
            return find_iter->second;
            LOG(warning) << "Requested object already loaded";
        }

        auto object = object_manager_->CreateObjectFromStorage(object_id, type);

        loaded_objects_.insert(make_pair(object_id, object));
		spatial_provider_->AddObject(object); // Add object to spatial indexing.
        return object;
    }

    shared_ptr<Object> GetObjectById(uint64_t object_id)
    {
        auto find_iter = loaded_objects_.find(object_id);

        if (find_iter == loaded_objects_.end())
        {
            return nullptr;
        }

        return find_iter->second;
    }

    void RemoveObjectById(uint64_t object_id)
    {
        auto find_iter = loaded_objects_.find(object_id);

        if (find_iter == loaded_objects_.end())
        {
            LOG(warning) << "Requested an invalid object";
        }

        RemoveObject(find_iter->second);
    }

    void RemoveObject(const shared_ptr<Object>& object)
    {
        auto scene = scene_manager_->GetScene(object->GetSceneId());
        if (scene)
        {
            scene->RemoveObject(object);
        }

		spatial_provider_->RemoveObject(object); // Remove the object from spatial indexing.

        StopControllingObject(object);

        loaded_objects_.unsafe_erase(loaded_objects_.find(object->GetObjectId()));

        auto contained_objects = object->GetContainedObjects();
        for_each(
            begin(contained_objects),
            end(contained_objects),
            [this] (const Object::ObjectMap::value_type& item)
        {
            RemoveObject(item.second);
        });
    }

    shared_ptr<ObjectController> StartControllingObject(const shared_ptr<Object>& object, shared_ptr<ConnectionClient> client)
    {
        shared_ptr<ObjectController> controller = nullptr;

        // If a controller already exists update it, otherwise create a new controller record.
        auto find_iter = controlled_objects_.find(object->GetObjectId());
        if (find_iter != controlled_objects_.end())
        {
            controller = find_iter->second;
            controller->SetRemoteClient(client);
        }
        else
        {
            controller = make_shared<ObjectController>(object, client);
            object->SetController(controller);

            controlled_objects_.insert(make_pair(object->GetObjectId(), controller));
        }

        auto connection_client = std::static_pointer_cast<ConnectionClient>(client);
        connection_client->SetController(controller);

        return controller;
    }

    void StopControllingObject(const shared_ptr<Object>& object)
    {
        auto find_iter = controlled_objects_.find(object->GetObjectId());

        if (find_iter == controlled_objects_.end())
        {
            return;
        }

        controlled_objects_.unsafe_erase(find_iter);
    }

    void RegisterControllerHandler(uint32_t handler_id, swganh::object::ObjControllerHandler&& handler)
    {
        auto find_iter = controller_handlers_.find(handler_id);

        if (find_iter != controller_handlers_.end())
        {
            // just return, we already have the handler registered
            return;
            //throw std::runtime_error("ObjControllerHandler already exists");
        }

        controller_handlers_.insert(make_pair(handler_id, move(handler)));
    }

    void UnregisterControllerHandler(uint32_t handler_id)
    {
        auto find_iter = controller_handlers_.find(handler_id);

        if (find_iter == controller_handlers_.end())
        {
            throw std::runtime_error("ObjControllerHandler does not exist");
        }

        controller_handlers_.unsafe_erase(find_iter);
    }

    void HandleObjControllerMessage(
        const shared_ptr<ConnectionClient>& client,
        ObjControllerMessage message)
    {
        auto find_iter = controller_handlers_.find(message.message_type);

        if (find_iter == controller_handlers_.end())
        {
            DLOG(warning) << "No handler registered to process the given message. " << message.data;
            return;
        }

        find_iter->second(client->GetController(), move(message));
    }

    void HandleSelectCharacter(
        const shared_ptr<ConnectionClient>& client,
        SelectCharacter message)
    {
        auto object = GetObjectById(message.character_id);

        if (!object)
        {
            object = LoadObjectById(message.character_id, creature::Creature::type);
        }

        /// @TODO REFACTOR Move this functionality out to a PlayerService
        auto contained = object->GetContainedObjects();

        for_each(
            begin(contained),
            end(contained),
            [] (Object::ObjectMap::value_type& object_entry)
        {
            auto player = dynamic_pointer_cast<player::Player>(object_entry.second);

            if (player)
            {
                player->RemoveStatusFlag(player::LD);
            }
        });
        // END todo

        StartControllingObject(object, client);

        auto scene = scene_manager_->GetScene(object->GetSceneId());

        if (!scene)
        {
            throw std::runtime_error("Invalid scene selected for object");
        }

        // CmdStartScene
        CmdStartScene start_scene;
        start_scene.ignore_layout = 0;
        start_scene.character_id = object->GetObjectId();

        start_scene.terrain_map = scene->GetTerrainMap();
        start_scene.position = object->GetPosition();
        start_scene.shared_race_template = object->GetTemplate();
        start_scene.galaxy_time = 0;
        client->SendTo(start_scene);

        // Add object to scene and send baselines
        scene->AddObject(object);
    }

	void SendToAll(ByteBuffer message)
	{
		for_each(begin(controlled_objects_), end(controlled_objects_), [=] (const pair<uint64_t, shared_ptr<ObjectController>>& pair) {
            auto controller = pair.second;
            controller->GetRemoteClient()->SendTo(message);
        });
	}

    void SendToAllInScene(ByteBuffer message, uint32_t scene_id)
    {
        for_each(begin(controlled_objects_), end(controlled_objects_), [=] (const pair<uint64_t, shared_ptr<ObjectController>>& pair) {
            auto controller = pair.second;
            if (controller->GetObject()->GetSceneId() == scene_id)
                controller->GetRemoteClient()->SendTo(message);
        });
    }

private:
    shared_ptr<ObjectManager> object_manager_;
    shared_ptr<SceneManager> scene_manager_;
    shared_ptr<MovementManager> movement_manager_;
    SwganhKernel* kernel_;
	ServerInterface* server_;
	shared_ptr<SpatialProviderInterface> spatial_provider_;

    ObjControllerHandlerMap controller_handlers_;

    Concurrency::concurrent_unordered_map<uint64_t, shared_ptr<Object>> loaded_objects_;
    Concurrency::concurrent_unordered_map<uint64_t, shared_ptr<ObjectController>> controlled_objects_;
};

}}  // namespace swganh::simulation

SimulationService::SimulationService(SwganhKernel* kernel)
    : impl_(new SimulationServiceImpl(kernel))
    , kernel_(kernel)
{}

SimulationService::~SimulationService()
{}

ServiceDescription SimulationService::GetServiceDescription()
{
    ServiceDescription service_description(
        "SimulationService",
        "simulation",
        "0.1",
        "127.0.0.1",
        0,
        0,
        0);

    return service_description;
}

void SimulationService::StartScene(const std::string& scene_label)
{
    impl_->GetSceneManager()->LoadSceneDescriptionsFromDatabase(kernel_->GetDatabaseManager()->getConnection("galaxy"));
    impl_->GetSceneManager()->StartScene(scene_label);
    // load factories
    RegisterObjectFactories();
}

void SimulationService::StopScene(const std::string& scene_label)
{
    impl_->GetSceneManager()->StopScene(scene_label);
}
void SimulationService::RegisterObjectFactories()
{
        auto db_manager = kernel_->GetDatabaseManager();
        auto event_dispatcher =  kernel_->GetEventDispatcher();
        impl_->GetObjectManager()->RegisterObjectType(0, make_shared<ObjectFactory>(db_manager, this, event_dispatcher));
        impl_->GetObjectManager()->RegisterObjectType(tangible::Tangible::type, make_shared<tangible::TangibleFactory>(db_manager, this, event_dispatcher));
        impl_->GetObjectManager()->RegisterObjectType(intangible::Intangible::type, make_shared<intangible::IntangibleFactory>(db_manager, this, event_dispatcher));
        impl_->GetObjectManager()->RegisterObjectType(creature::Creature::type, make_shared<creature::CreatureFactory>(db_manager, this, event_dispatcher));
        impl_->GetObjectManager()->RegisterObjectType(player::Player::type, make_shared<player::PlayerFactory>(db_manager, this, event_dispatcher));
        // register message builders
        impl_->GetObjectManager()->RegisterMessageBuilder(tangible::Tangible::type, make_shared<tangible::TangibleMessageBuilder>(event_dispatcher));
        impl_->GetObjectManager()->RegisterMessageBuilder(creature::Creature::type, make_shared<creature::CreatureMessageBuilder>(event_dispatcher));
        impl_->GetObjectManager()->RegisterMessageBuilder(player::Player::type, make_shared<player::PlayerMessageBuilder>(event_dispatcher));
}

void SimulationService::PersistObject(uint64_t object_id)
{
    impl_->PersistObject(object_id);
}
void SimulationService::PersistRelatedObjects(uint64_t parent_object_id)
{
	impl_->PersistRelatedObjects(parent_object_id);
}
shared_ptr<Object> SimulationService::LoadObjectById(uint64_t object_id)
{
    return impl_->LoadObjectById(object_id);
}
shared_ptr<Object> SimulationService::LoadObjectById(uint64_t object_id, uint32_t type)
{
    return impl_->LoadObjectById(object_id, type);
}

shared_ptr<Object> SimulationService::GetObjectById(uint64_t object_id)
{
    return impl_->GetObjectById(object_id);
}

void SimulationService::RemoveObjectById(uint64_t object_id)
{
    impl_->RemoveObjectById(object_id);
}

void SimulationService::RemoveObject(const shared_ptr<Object>& object)
{
    impl_->RemoveObject(object);
}

shared_ptr<ObjectController> SimulationService::StartControllingObject(
    const shared_ptr<Object>& object,
    shared_ptr<ConnectionClient> client)
{
    return impl_->StartControllingObject(object, client);
}

void SimulationService::StopControllingObject(const shared_ptr<Object>& object)
{
    impl_->StopControllingObject(object);
}

void SimulationService::RegisterControllerHandler(
    uint32_t handler_id,
    swganh::object::ObjControllerHandler&& handler)
{
    impl_->RegisterControllerHandler(handler_id, move(handler));
}

void SimulationService::UnregisterControllerHandler(uint32_t handler_id)
{
    impl_->UnregisterControllerHandler(handler_id);
}

void SimulationService::SendToAll(ByteBuffer message)
{
    impl_->SendToAll(message);
}

void SimulationService::SendToAllInScene(ByteBuffer message, uint32_t scene_id)
{
    impl_->SendToAllInScene(message, scene_id);
}

void SimulationService::Start()
{
	auto connection_service = kernel_->GetServiceManager()->GetService<ConnectionService>("ConnectionService");

    connection_service->RegisterMessageHandler(
        &SimulationServiceImpl::HandleSelectCharacter, impl_.get());

    connection_service->RegisterMessageHandler(
        &SimulationServiceImpl::HandleObjControllerMessage, impl_.get());

    RegisterControllerHandler(
        &MovementManager::HandleDataTransform, impl_->GetMovementManager());

    RegisterControllerHandler(
        &MovementManager::HandleDataTransformWithParent, impl_->GetMovementManager());

    
	auto command_service = kernel_->GetServiceManager()->GetService<swganh::command::CommandService>("CommandService");

    command_service->AddCommandCreator("burstrun", swganh::command::PythonCommandCreator("commands.burstrun", "BurstRunCommand"));
}
