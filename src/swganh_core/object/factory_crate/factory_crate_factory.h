// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh_core/object/tangible/tangible_factory.h"

namespace swganh {
namespace object {

	class ObjectManager;



	class FactoryCrate;

    class FactoryCrateFactory : public swganh::object::TangibleFactory
    {
    public:
		typedef FactoryCrate ObjectType;

        FactoryCrateFactory(swganh::database::DatabaseManagerInterface* db_manager,
            swganh::EventDispatcher* event_dispatcher);

		virtual void RegisterEventHandlers(){}

    };


}}  // namespace swganh::object
