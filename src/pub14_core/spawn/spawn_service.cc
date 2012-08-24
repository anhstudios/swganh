#include "spawn_service.h"

#include <algorithm>
#include <memory>

#include "anh/logger.h"

#include "anh/service/service_manager.h"

#include "pub14_core/simulation/simulation_service.h"
#include "pub14_core/simulation/scene_events.h"

#include "swganh/object/object_manager.h"
#include "swganh/object/object.h"

#include "swganh/tre/resource_manager.h"
#include "swganh/tre/visitors/snapshot/ws_visitor.h"

using namespace anh::service;
using namespace swganh::app;
using namespace swganh::simulation;
using namespace swganh::tre;

using namespace swganh_core::spawn;
using namespace swganh_core::simulation;

SpawnService::SpawnService(SwganhKernel* kernel) : kernel_(kernel)
{
	kernel_->GetEventDispatcher()->Subscribe("SceneManager:NewScene", [&] (const std::shared_ptr<anh::EventInterface>& newEvent)
	{
		auto real_event = std::static_pointer_cast<swganh_core::simulation::NewSceneEvent>(newEvent);
		
		auto simulation_service = kernel_->GetServiceManager()->GetService<SimulationServiceInterface>("SimulationService");
		auto object_manager = simulation_service->GetObjectManager();
		
		//Load objects from snapshot
		
		std::string snapshot_filename = "snapshot/"+real_event->scene_label+".ws";
		auto visitor = kernel_->GetResourceManager()->getResourceByName(snapshot_filename, WS_VISITOR, false);
		auto snapshot_visitor = std::static_pointer_cast<WsVisitor>(visitor);
			
		auto chunks = snapshot_visitor->chunks();
		std::for_each(chunks.begin(), chunks.end(), [&] (WsVisitor::CHUNK& chunk) {
			try
			{
				auto object = object_manager->CreateObjectFromTemplate(snapshot_visitor->name(chunk.name_id), chunk.id);
				
				//LOG(warning) << "LOADING OBJECT: " << snapshot_visitor->name(chunk.name_id);
				
				object->SetEventDispatcher(kernel_->GetEventDispatcher());
				object->SetPosition(chunk.location);
				object->SetOrientation(chunk.orientation);
				object->SetSceneId(real_event->scene_id);
					
				//@Todo: Set scale
				
				if(chunk.parent_id == 0)
				{
					//Put it into the scene
					simulation_service->TransferObjectToScene(object, real_event->scene_label);
				}
				else
				{
					//It has a parent, so get it's parent and put it into that
					auto parent = object_manager->GetObjectById(chunk.parent_id);
					if(parent != nullptr)
					{
						parent->AddObject(nullptr, object);
					}
				}
			}
			catch(...)
			{
				LOG(warning) << "Failed to load files from snapshot for: " << real_event->scene_label;
			}
		});		
	});

	/* Dont need this now, but we might later.
	
	kernel_->GetEventDispatcher()->Subscribe("SceneManager:DestroyScene", [&] (const std::shared_ptr<anh::EventInterface>& newEvent)
	{
		auto real_event = std::static_pointer_cast<swganh_core::simulation::DestroySceneEvent>(newEvent);
		
	});*/
}

SpawnService::~SpawnService()
{
}

ServiceDescription SpawnService::GetServiceDescription()
{
    ServiceDescription service_description(
        "SpawnService",
        "spawn",
        "0.1",
        "127.0.0.1",
        0,
        0,
        0);

    return service_description;
}