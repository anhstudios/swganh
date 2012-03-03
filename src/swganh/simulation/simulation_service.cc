
#include "swganh/simulation/simulation_service.h"

#include <boost/algorithm/string.hpp>

#include "anh/crc.h"
#include "anh/service/service_manager.h"
#include "anh/database/database_manager.h"

#include "swganh/app/swganh_kernel.h"

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

#include "swganh/simulation/scene_manager.h"
#include "swganh/messages/cmd_start_scene.h"
#include "swganh/messages/cmd_scene_ready.h"
#include "swganh/messages/obj_controller_message.h"
#include "swganh/messages/update_containment_message.h"

#include "swganh/simulation/movement_manager.h"

using namespace std;
using namespace swganh::connection;
using namespace swganh::messages;
using namespace swganh::network;
using namespace swganh::object;
using namespace swganh::simulation;

using anh::app::KernelInterface;
using anh::service::ServiceDescription;
using swganh::base::BaseService;

namespace swganh {
namespace simulation {

class SimulationServiceImpl {
public:
    SimulationServiceImpl(KernelInterface* kernel)
        : kernel_(kernel)
    {
    }

    const shared_ptr<ObjectManager>& GetObjectManager()
    {
        if (!object_manager_)
        {
            object_manager_ = make_shared<ObjectManager>();
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

    const shared_ptr<MovementManager>& GetMovementManager()
    {
        if (!movement_manager_)
        {
            movement_manager_ = make_shared<MovementManager>(kernel_->GetEventDispatcher());
        }

        return movement_manager_;
    }

    void PersistObject(uint64_t object_id)
    {
        auto find_iter = loaded_objects_.find(object_id);

        if (find_iter == loaded_objects_.end())
        {
            BOOST_LOG_TRIVIAL(warning) << "Nothing to persist, no object saved";
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
            BOOST_LOG_TRIVIAL(warning) << "Nothing to persist, no object saved";
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
				BOOST_LOG_TRIVIAL(warning) << "Persist inner container recursively:" << pair.first;
				PersistRelatedObjects(pair.first);
			}
			else
			{
				BOOST_LOG_TRIVIAL(warning) << "Persist inner container:" << pair.first;
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
            //throw swganh::object::InvalidObject("Requested object already loaded");
        }
        
        auto object = object_manager_->CreateObjectFromStorage(object_id);

        loaded_objects_.insert(make_pair(object_id, object));

        return object;
    }
    shared_ptr<Object> LoadObjectById(uint64_t object_id, uint32_t type)
    {
        auto find_iter = loaded_objects_.find(object_id);

        if (find_iter != loaded_objects_.end())
        {
            return find_iter->second;
            //throw swganh::object::InvalidObject("Requested object already loaded");
        }
        
        auto object = object_manager_->CreateObjectFromStorage(object_id, type);

        loaded_objects_.insert(make_pair(object_id, object));

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
            throw swganh::object::InvalidObject("Requested an invalid object");
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
        const ObjControllerMessage& message)
    {
        auto find_iter = controller_handlers_.find(message.header);
        
        if (find_iter == controller_handlers_.end())
        {
            throw std::runtime_error("No handler registered to process the given message.");
        }
        
        find_iter->second(client->GetController(), message);
    }
    
    void HandleSelectCharacter(
        const shared_ptr<ConnectionClient>& client, 
        const SelectCharacter& message)
    {
        auto object = GetObjectById(message.character_id);
        
        if (!object)
        {
            object = LoadObjectById(message.character_id, creature::Creature::type);
        }

        // @TODO REFACTOR Move this functionality out to a PlayerService
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
        // END TODO

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
        client->SendMessage(start_scene);

        // Add object to scene and send baselines
        scene->AddObject(object);
    }

private:
    shared_ptr<ObjectManager> object_manager_;
    shared_ptr<SceneManager> scene_manager_;
    shared_ptr<MovementManager> movement_manager_;
    KernelInterface* kernel_;

    ObjControllerHandlerMap controller_handlers_;

    Concurrency::concurrent_unordered_map<uint64_t, shared_ptr<Object>> loaded_objects_;
    Concurrency::concurrent_unordered_map<uint64_t, shared_ptr<ObjectController>> controlled_objects_;
};

}}  // namespace swganh::simulation

SimulationService::SimulationService(KernelInterface* kernel)
    : BaseService(kernel) 
    , impl_(new SimulationServiceImpl(kernel))
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
    impl_->GetSceneManager()->LoadSceneDescriptionsFromDatabase(kernel()->GetDatabaseManager()->getConnection("galaxy"));
    impl_->GetSceneManager()->StartScene(scene_label);
    // load factories
    RegisterObjectFactories(kernel());
}

void SimulationService::StopScene(const std::string& scene_label)
{
    impl_->GetSceneManager()->StopScene(scene_label);
}
void SimulationService::RegisterObjectFactories(anh::app::KernelInterface* kernel)
{
        auto db_manager = kernel->GetDatabaseManager();
        impl_->GetObjectManager()->RegisterObjectType(0, make_shared<ObjectFactory>(db_manager, this));
        impl_->GetObjectManager()->RegisterObjectType(tangible::Tangible::type, make_shared<tangible::TangibleFactory>(db_manager, this));
        impl_->GetObjectManager()->RegisterObjectType(intangible::Intangible::type, make_shared<intangible::IntangibleFactory>(db_manager, this));
        impl_->GetObjectManager()->RegisterObjectType(creature::Creature::type, make_shared<creature::CreatureFactory>(db_manager, this));
        impl_->GetObjectManager()->RegisterObjectType(player::Player::type, make_shared<player::PlayerFactory>(db_manager, this));
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

void SimulationService::onStart()
{
	auto connection_service = std::static_pointer_cast<ConnectionService>(kernel()->GetServiceManager()->GetService("ConnectionService"));
        
    connection_service->RegisterMessageHandler(
        &SimulationServiceImpl::HandleSelectCharacter, impl_.get());
    
    connection_service->RegisterMessageHandler(
        &SimulationServiceImpl::HandleObjControllerMessage, impl_.get());

    RegisterControllerHandler(0x00000071, [this] (
        const std::shared_ptr<ObjectController>& controller, 
        const swganh::messages::ObjControllerMessage& message) 
    {
        this->impl_->GetMovementManager()->HandleDataTransform(controller, message);
    });
    
    RegisterControllerHandler(0x000000F1, [this] (
        const std::shared_ptr<ObjectController>& controller, 
        const swganh::messages::ObjControllerMessage& message) 
    {
        this->impl_->GetMovementManager()->HandleDataTransformWithParent(controller, message);
    });
}
