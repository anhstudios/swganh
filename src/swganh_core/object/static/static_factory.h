// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh_core/object/object_factory.h"

namespace swganh {
namespace database {
class DatabaseManagerInterface;
}} // swganh::database

namespace sql {
class Statement;
}

namespace swganh {
namespace object {


	class Static;
    class StaticFactory : public swganh::object::ObjectFactory
    {
    public:
		typedef Static ObjectType;

		 StaticFactory(swganh::app::SwganhKernel* kernel);

		virtual void PersistChangedObjects(){}
        virtual uint32_t PersistObject(const std::shared_ptr<swganh::object::Object>& object);

        void DeleteObjectFromStorage(const std::shared_ptr<swganh::object::Object>& object);

        std::shared_ptr<swganh::object::Object> CreateObjectFromStorage(uint64_t object_id);

        std::shared_ptr<swganh::object::Object> CreateObject();
    };

}}  // namespace swganh::object
