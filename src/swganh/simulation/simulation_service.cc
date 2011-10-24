
#include "swganh/simulation/simulation_service.h"

#include <boost/algorithm/string.hpp>

#include "anh/crc.h"
#include "anh/service/service_manager.h"
#include "anh/database/database_manager.h"

#include "swganh/app/swganh_kernel.h"

#include "swganh/connection/connection_client.h"
#include "swganh/connection/connection_service.h"

#include "swganh/messages/select_character.h"

#include "swganh/network/remote_client.h"

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
            movement_manager_ = make_shared<MovementManager>();
        }

        return movement_manager_;
    }

    void RegisterObjectFactories(anh::app::KernelInterface* kernel)
    {
        auto db_manager = kernel->GetDatabaseManager();
        GetObjectManager()->RegisterObjectType(0, make_shared<ObjectFactory>(db_manager));
        GetObjectManager()->RegisterObjectType(tangible::Tangible::type, make_shared<tangible::TangibleFactory>(db_manager));
        GetObjectManager()->RegisterObjectType(intangible::Intangible::type, make_shared<intangible::IntangibleFactory>(db_manager));
        GetObjectManager()->RegisterObjectType(creature::Creature::type, make_shared<creature::CreatureFactory>(db_manager));
        GetObjectManager()->RegisterObjectType(player::Player::type, make_shared<player::PlayerFactory>(db_manager));
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
    const shared_ptr<Object>& GetObjectById(uint64_t object_id)
    {
        auto find_iter = loaded_objects_.find(object_id);

        if (find_iter == loaded_objects_.end())
        {
            throw swganh::object::InvalidObject("Requested an invalid object");
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

        auto scene = scene_manager_->GetScene(find_iter->second->GetSceneId());
        if (scene)
        {
            scene->RemoveObject(find_iter->second);
        }

        StopControllingObject(find_iter->second);

        loaded_objects_.erase(find_iter);
    }

    shared_ptr<ObjectController> StartControllingObject(const shared_ptr<Object>& object, shared_ptr<RemoteClient> client)
    {
        auto find_iter = controlled_objects_.find(object->GetObjectId());

        if (find_iter != controlled_objects_.end())
        {
			return find_iter->second;
            //throw swganh::object::InvalidObject("Object already has a controller");
        }
        
        auto controller = make_shared<ObjectController>(object, client);
        object->SetController(controller);

        auto connection_client = std::static_pointer_cast<ConnectionClient>(client);
        connection_client->SetController(controller);

        controlled_objects_.insert(make_pair(object->GetObjectId(), controller));

        return controller;
    }

    void StopControllingObject(const shared_ptr<Object>& object)
    {
        auto find_iter = controlled_objects_.find(object->GetObjectId());

        if (find_iter == controlled_objects_.end())
        {
            throw swganh::object::InvalidObject("Object has no controller");
        }
        
        controlled_objects_.erase(find_iter);
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
        shared_ptr<ConnectionClient> client,
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
        shared_ptr<ConnectionClient> client, 
        const SelectCharacter& message)
    {
        // character_id = player
		auto object = LoadObjectById(message.character_id, creature::Creature::type);
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
        client->GetSession()->SendMessage(start_scene);

        // Add object to scene and send baselines
		scene->AddObject(object);
    }

private:
    shared_ptr<ObjectManager> object_manager_;
    shared_ptr<SceneManager> scene_manager_;
    shared_ptr<MovementManager> movement_manager_;

    ObjControllerHandlerMap controller_handlers_;

    map<uint64_t, shared_ptr<Object>> loaded_objects_;
    map<uint64_t, shared_ptr<ObjectController>> controlled_objects_;
};

}}  // namespace swganh::simulation

SimulationService::SimulationService(KernelInterface* kernel)
    : BaseService(kernel) 
    , impl_(new SimulationServiceImpl)
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
    impl_->RegisterObjectFactories(kernel);
}

shared_ptr<Object> SimulationService::LoadObjectById(uint64_t object_id)
{
    return impl_->LoadObjectById(object_id);
}
shared_ptr<Object> SimulationService::LoadObjectById(uint64_t object_id, uint32_t type)
{
    return impl_->LoadObjectById(object_id, type);
}

const shared_ptr<Object>& SimulationService::GetObjectById(uint64_t object_id)
{
    return impl_->GetObjectById(object_id);
}

void SimulationService::RemoveObjectById(uint64_t object_id)
{
    impl_->RemoveObjectById(object_id);
}

shared_ptr<ObjectController> SimulationService::StartControllingObject(
    const shared_ptr<Object>& object, 
    shared_ptr<RemoteClient> client)
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
        
    connection_service->RegisterMessageHandler<SelectCharacter>([=] (
        shared_ptr<ConnectionClient> client, 
        const SelectCharacter& message)
    {
        impl_->HandleSelectCharacter(client, message);
    });

    connection_service->RegisterMessageHandler<ObjControllerMessage>([=] (
        shared_ptr<ConnectionClient> client, 
        const ObjControllerMessage& message)
    {
        impl_->HandleObjControllerMessage(client, message);
    });


    RegisterControllerHandler(0x00000071, [this] (
        const std::shared_ptr<ObjectController>& controller, 
        const swganh::messages::ObjControllerMessage& message) 
    {
        impl_->GetMovementManager()->HandleDataTransform(controller, message);
    });
    
    RegisterControllerHandler(0x000000F1, [this] (
        const std::shared_ptr<ObjectController>& controller, 
        const swganh::messages::ObjControllerMessage& message) 
    {
        impl_->GetMovementManager()->HandleDataTransformWithParent(controller, message);
    });
}
