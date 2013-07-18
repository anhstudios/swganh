// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "structure_service_interface.h"
#include "swganh/app/swganh_kernel.h"

namespace swganh
{

namespace structure
{
	class StructureService : public StructureServiceInterface
	{
	public:
        StructureService(swganh::app::SwganhKernel* kernel);
        ~StructureService();
        
        void Initialize();
        
		void EnterStructurePlacementMode(std::shared_ptr<swganh::object::Object> player,
            uint64_t deed_id, std::string building_template_iff);

		void PlaceStructureByDeed(std::shared_ptr<swganh::object::Object> owner,
			uint64_t deed_id, float x, float y, float direction);

    private:
        swganh::app::SwganhKernel* kernel_;
    };
}
}