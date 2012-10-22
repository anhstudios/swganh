// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh_core/object/object_factory.h"

#include <unordered_map>

namespace swganh {
namespace database {
class DatabaseManagerInterface;
}} // swganh::database

namespace sql {
class Statement;
}

namespace swganh {
namespace object {


    class Tangible;
    class TangibleFactory : public swganh::object::ObjectFactory
    {
    public:
        typedef Tangible ObjectType;

        TangibleFactory(swganh::app::SwganhKernel* kernel);
        virtual uint32_t PersistObject(const std::shared_ptr<swganh::object::Object>& object);

        void DeleteObjectFromStorage(const std::shared_ptr<swganh::object::Object>& object);
		virtual void PersistChangedObjects();
        std::shared_ptr<swganh::object::Object> CreateObjectFromStorage(uint64_t object_id);
        void CreateTangible(const std::shared_ptr<Tangible>& tangible, const std::shared_ptr<sql::Statement>& statement);

        std::shared_ptr<swganh::object::Object> CreateObject();
        
        virtual void RegisterEventHandlers();
    private:
    };

}}  // namespace swganh::object
