// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_OBJECT_FACTORY_CRATE_FACTORY_CRATE_FACTORY_H_
#define SWGANH_OBJECT_FACTORY_CRATE_FACTORY_CRATE_FACTORY_H_

#include "swganh/object/tangible/tangible_factory.h"

namespace swganh {
namespace object {

	class ObjectManager;

namespace factory_crate {

	class FactoryCrate;

    class FactoryCrateFactory : public swganh::object::tangible::TangibleFactory
    {
    public:
		typedef FactoryCrate ObjectType;

        FactoryCrateFactory(anh::database::DatabaseManagerInterface* db_manager,
            anh::EventDispatcher* event_dispatcher);

        uint32_t PersistObject(const std::shared_ptr<swganh::object::Object>& object);

        void DeleteObjectFromStorage(const std::shared_ptr<swganh::object::Object>& object);

        std::shared_ptr<swganh::object::Object> CreateObjectFromStorage(uint64_t object_id);

        std::shared_ptr<swganh::object::Object> CreateObjectFromTemplate(const std::string& template_name, bool db_persisted=true, bool db_initialized=true);

		virtual void RegisterEventHandlers(){}

    };

}}}  // namespace swganh::object::building

#endif  // SWGANH_OBJECT_BUILDING_BUILDING_FACTORY_H_
