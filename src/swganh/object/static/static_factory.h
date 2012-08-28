// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh/object/object_factory.h"

namespace anh {
namespace database {
class DatabaseManagerInterface;
}} // anh::database

namespace sql {
class Statement;
}

namespace swganh {
namespace object {
namespace static_object {

	class Static;
    class StaticFactory : public swganh::object::ObjectFactory
    {
    public:
		typedef Static ObjectType;

		 StaticFactory(anh::database::DatabaseManagerInterface* db_manager,
            anh::EventDispatcher* event_dispatcher);

        uint32_t PersistObject(const std::shared_ptr<swganh::object::Object>& object);

        void DeleteObjectFromStorage(const std::shared_ptr<swganh::object::Object>& object);

        std::shared_ptr<swganh::object::Object> CreateObjectFromStorage(uint64_t object_id);

        std::shared_ptr<swganh::object::Object> CreateObjectFromTemplate(const std::string& template_name, bool db_persisted=true, bool db_initialized=true);
    };

}}}  // namespace swganh::object::static
