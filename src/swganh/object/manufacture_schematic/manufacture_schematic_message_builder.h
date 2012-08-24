// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_OBJECT_MANUFACTURE_SCHEMATIC_MANUFACTURE_SCHEMATIC_MESSAGE_BUILDER_H_
#define SWGANH_OBJECT_MANUFACTURE_SCHEMATIC_MANUFACTURE_SCHEMATIC_MESSAGE_BUILDER_H_

#include "swganh/object/intangible/intangible_message_builder.h"

#include "manufacture_schematic.h"

namespace swganh {
namespace object {
namespace manufacture_schematic {
    class ManufactureSchematicMessageBuilder : public swganh::object::intangible::IntangibleMessageBuilder
    {
    public:
        ManufactureSchematicMessageBuilder(anh::EventDispatcher* dispatcher) :
            IntangibleMessageBuilder(dispatcher)
        {
            RegisterEventHandlers();
        }
        virtual void SendBaselines(const std::shared_ptr<ManufactureSchematic>& manufacture_schematic, const std::shared_ptr<anh::observer::ObserverInterface>& observer);
        virtual void RegisterEventHandlers();
		//deltas
		static void BuildDelta(const std::shared_ptr<ManufactureSchematic>& manufacture_schematic);

        // baselines
        static swganh::messages::BaselinesMessage BuildBaseline3(const std::shared_ptr<ManufactureSchematic>& manufacture_schematic);
        static swganh::messages::BaselinesMessage BuildBaseline6(const std::shared_ptr<ManufactureSchematic>& manufacture_schematic);
		static swganh::messages::BaselinesMessage BuildBaseline6(const std::shared_ptr<ManufactureSchematic>& manufacture_schematic);
	private:
		typedef anh::ValueEvent<std::shared_ptr<ManufactureSchematic>> ManufactureSchematicEvent;
    };

}}}  // swganh::object::player

#endif  // SWGANH_OBJECT_PLAYER_PLAYER_MESSAGE_BUILDER_H_
