#include "structure_service.h"
#include "swganh_core/object/object.h"

#include "swganh_core/messages/enter_structure_placement_mode_message.h"

using namespace swganh::structure;
using namespace swganh::messages;

StructureService::StructureService(swganh::app::SwganhKernel* kernel)
    : kernel_(kernel)
{
	SetServiceDescription(service::ServiceDescription(
		"Structure Service",
		"structure",
		"0.1",
		"127.0.0.1",
		0,
		0,
		0));
}

StructureService::~StructureService()
{
}

void StructureService::Initialize()
{
}

void StructureService::EnterStructurePlacementMode(std::shared_ptr<swganh::object::Object> player,
    uint64_t deed_id, std::string building_template_iff)
{
    EnterStructurePlacementModeMessage msg;
    msg.deed_id = deed_id;
    msg.object_file_path = building_template_iff;
	if (auto controller = player->GetController())
	{
		controller->Notify(&msg);
	}
}

void StructureService::PlaceStructureByDeed(std::shared_ptr<swganh::object::Object> owner,
	uint64_t deed_id, float x, float y, float direction)
{
	printf("NOTHING TO GIVE AND NO ONE TO BLAME.\n");
	
	//Look up deed

	//Confirm owner & Delete

	//Get Building iff and create

	//Look up information about placement of terminal and sign
	// & create both

	//Asynchronously hit the terrain service for height data.
		//Insert into world.
}