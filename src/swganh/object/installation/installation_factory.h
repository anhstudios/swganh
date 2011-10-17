
#ifndef SWGANH_OBJECT_INSTALLATION_INSTALLATION_FACTORY_H_
#define SWGANH_OBJECT_INSTALLATION_INSTALLATION_FACTORY_H_

#include "swganh/object/object_factory_interface.h"

namespace swganh {
namespace object {
namespace installation {

    class InstallationFactory : public swganh::object::ObjectFactoryInterface
    {
    public:
        void LoadTemplates();

        bool HasTemplate(const std::string& template_name);

        void PersistObject(const std::shared_ptr<swganh::object::Object>& object);

        void DeleteObjectFromStorage(const std::shared_ptr<swganh::object::Object>& object);

        std::shared_ptr<swganh::object::Object> CreateObjectFromStorage(uint64_t object_id);

        std::shared_ptr<swganh::object::Object> CreateObjectFromTemplate(const std::string& template_name);
    };

}}}  // namespace swganh::object::installation

#endif  // SWGANH_OBJECT_INSTALLATION_INSTALLATION_FACTORY_H_
