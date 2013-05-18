// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh_core/object/intangible/intangible_factory.h"

namespace swganh {
namespace object {


	class Mission;
    class MissionFactory : public swganh::object::IntangibleFactory
    {
    public:
		typedef Mission ObjectType;

		MissionFactory(swganh::app::SwganhKernel* kernel);
        
        virtual void LoadFromStorage(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Object>& object, boost::unique_lock<boost::mutex>& lock);

        virtual uint32_t PersistObject(const std::shared_ptr<swganh::object::Object>& object, boost::unique_lock<boost::mutex>& lock, bool persist_inherited = false);
		
        virtual void PersistChangedObjects(){}

        void DeleteObjectFromStorage(const std::shared_ptr<swganh::object::Object>& object);

        std::shared_ptr<swganh::object::Object> CreateObject();
    };

}}  // namespace swganh::object
