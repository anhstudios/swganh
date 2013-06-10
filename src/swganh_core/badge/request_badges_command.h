// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <memory>

#include "swganh_core/command/base_swg_command.h"

namespace swganh {

namespace equipment {
	class EquipmentService;
} // namespace swganh::equipment

namespace badge {

    class BadgeService;

	/**
	* An command implementation for request badges
	*/
    class RequestBadgesCommand : public swganh::command::BaseSwgCommand
    {
    public:

        virtual std::string GetCommandName() const { return "requestbadges"; }

		/**
			Creates a new instance
		*/
        void Initialize(
            swganh::app::SwganhKernel* kernel,
            const swganh::command::CommandProperties& properties);

		/**
		* Runs this command
		*/
        virtual void Run();

    private:
		swganh::equipment::EquipmentService* equipment_service_;
    };

}}
