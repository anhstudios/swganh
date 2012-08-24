
#include "anh/byte_buffer.h"
#include "resource_manager.h"

#include "iff/iff.h"

#include "visitors/datatables/datatable_visitor.h"
#include "visitors/objects/object_visitor.h"
#include "visitors/portals/pob_visitor.h"
#include "visitors/slots/slot_arrangement_visitor.h"
#include "visitors/slots/slot_descriptor_visitor.h"
#include "visitors/slots/slot_definition_visitor.h"
#include "visitors/snapshot/ws_visitor.h"
#include "visitors/terrain/terrain_visitor.h"
//#include "visitors/terrain/layer_visitor.h"

using namespace swganh::tre;

ResourceManager::ResourceManager(std::shared_ptr<TreArchive> archive) 
	: archive_(archive)
{
}

std::shared_ptr<VisitorInterface> ResourceManager::getResourceByName(const std::string& name, VisitorType type, bool is_cached) 
{
	std::shared_ptr<VisitorInterface> result = nullptr;
	auto itr = loadedResources_.find(name);
	if(itr == loadedResources_.end()) {
		std::string nameCopy(name); 
		//Fill the result
		switch(type)
		{
		case POB_VISITOR:
			result = std::make_shared<PobVisitor>();
			break;
		case WS_VISITOR:
			result = std::make_shared<WsVisitor>();
			break;
		case OIFF_VISITOR:
			result = std::make_shared<ObjectVisitor>();
			break;
		case SLOT_ARRANGEMENT_VISITOR:
			result = std::make_shared<SlotArrangementVisitor>();
			break;
		case SLOT_DESCRIPTOR_VISITOR:
			result = std::make_shared<SlotDescriptorVisitor>();
			break;
		case SLOT_DEFINITION_VISITOR:
			result = std::make_shared<SlotDefinitionVisitor>();
			break;
		case DATATABLE_VISITOR:
			result = std::make_shared<DatatableVisitor>();
			break;
		case TRN_VISITOR:
			result = std::make_shared<TerrainVisitor>();
			break;
		//case LAY_VISITOR:
			//result = std::make_shared<LayerVisitor>();
			//break;
		default:
			return nullptr;
		}

		//Load it
		iff_file iff(archive_->GetResource(name), result);
		if(is_cached)
		{
			loadedResources_.insert(std::make_pair<std::string, std::shared_ptr<VisitorInterface>>(std::move(nameCopy), std::move(result)));
			itr = loadedResources_.find(name);
			return itr->second;
		}
	}
	return result;
}