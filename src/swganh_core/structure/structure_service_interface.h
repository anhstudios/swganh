// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh/service/service_interface.h"
#include "swganh/app/swganh_kernel.h"

namespace swganh
{
namespace object
{
    class Object; 
}

namespace structure
{
	class StructureServiceInterface : public swganh::service::BaseService
	{
	public:
		virtual ~StructureServiceInterface() {}
        
        virtual void Initialize() = 0;
        
        virtual void EnterStructurePlacementMode(std::shared_ptr<swganh::object::Object> player,
            uint64_t deed_id, std::string building_template_iff) = 0;

		virtual void PlaceStructureByDeed(std::shared_ptr<swganh::object::Object> owner,
			uint64_t deed_id, float x, float y, float direction) = 0;
    };
}
}