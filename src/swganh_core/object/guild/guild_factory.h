// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh_core/object/object_factory.h"

namespace swganh {
namespace object {


	class Guild;
    class GuildFactory : public swganh::object::ObjectFactory
    {
    public:
		GuildFactory(swganh::app::SwganhKernel* kernel);
		typedef Guild ObjectType;
        
        virtual void LoadFromStorage(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Object>& object, boost::unique_lock<boost::mutex>& lock);

        virtual uint32_t PersistObject(const std::shared_ptr<swganh::object::Object>& object, boost::unique_lock<boost::mutex>& lock, bool persist_inherited = false);

        void DeleteObjectFromStorage(const std::shared_ptr<swganh::object::Object>& object);
		virtual void PersistChangedObjects(){}

        std::shared_ptr<swganh::object::Object> CreateObjectFromTemplate(const std::string& template_name, bool db_persisted=true, bool db_initialized=true);
    };

}}  // namespace swganh::object
