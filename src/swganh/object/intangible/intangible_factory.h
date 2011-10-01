
#ifndef SWGANH_OBJECT_INTANGIBLE_INTANGIBLE_FACTORY_H_
#define SWGANH_OBJECT_INTANGIBLE_INTANGIBLE_FACTORY_H_

#include "swganh/object/object_factory_interface.h"
#include <unordered_map>

namespace anh {
namespace database {
class DatabaseManagerInterface;
}} // anh::database

namespace swganh {
namespace object {
namespace intangible {
    class Intangible;

    class IntangibleFactory : public swganh::object::ObjectFactoryInterface
    {
    public:
        IntangibleFactory(const std::shared_ptr<anh::database::DatabaseManagerInterface>& db_manager);
        void LoadTemplates();

        bool HasTemplate(const std::string& template_name);

        void PersistObject(const std::shared_ptr<swganh::object::Object>& object);

        void DeleteObjectFromStorage(const std::shared_ptr<swganh::object::Object>& object);

        std::shared_ptr<swganh::object::Object> CreateObjectFromStorage(uint64_t object_id);

        std::shared_ptr<swganh::object::Object> CreateObjectFromTemplate(const std::string& template_name);
    private:
        IntangibleFactory(){}
        std::unordered_map<std::string, std::shared_ptr<Intangible>>::iterator GetTemplateIter_(const std::string& template_name);
        std::shared_ptr<anh::database::DatabaseManagerInterface> db_manager_;
        std::unordered_map<std::string, std::shared_ptr<Intangible>> intangible_templates_;
    };

}}}  // namespace swganh::object::intangible

#endif  // SWGANH_OBJECT_INTANGIBLE_INTANGIBLE_FACTORY_H_
