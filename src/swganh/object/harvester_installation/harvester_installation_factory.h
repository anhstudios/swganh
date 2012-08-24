// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_OBJECT_HARVESTER_INSTALLATION_HARVESTER_INSTALLATION_FACTORY_H_
#define SWGANH_OBJECT_HARVESTER_INSTALLATION_HARVESTER_INSTALLATION_FACTORY_H_

#include "swganh/object/installation/installation_factory.h"

namespace swganh {
namespace object {
namespace harvester_installation {

	class HarvesterInstallation;
    class HarvesterInstallationFactory : public swganh::object::installation::InstallationFactory
    {
    public:
		typedef HarvesterInstallation ObjectType;

        HarvesterInstallationFactory(anh::database::DatabaseManagerInterface* db_manager, anh::EventDispatcher* event_dispatcher);

        uint32_t PersistObject(const std::shared_ptr<swganh::object::Object>& object);

        void DeleteObjectFromStorage(const std::shared_ptr<swganh::object::Object>& object);

        std::shared_ptr<swganh::object::Object> CreateObjectFromStorage(uint64_t object_id);

        std::shared_ptr<swganh::object::Object> CreateObjectFromTemplate(const std::string& template_name, bool db_persisted=true, bool db_initialized=true);
    };

}}}  // namespace swganh::object::installation

#endif  // SWGANH_OBJECT_INSTALLATION_HARVESTER_INSTALLATION_FACTORY_H_
