
#ifndef SWGANH_OBJECT_TANGIBLE_TANGIBLE_FACTORY2_H_
#define SWGANH_OBJECT_TANGIBLE_TANGIBLE_FACTORY2_H_

#include "swganh/object/object_factory.h"

#include <unordered_map>

namespace anh {
namespace database {
class DatabaseManagerInterface;
}} // anh::database

namespace swganh {
namespace object {
namespace tangible {

    class Tangible;

    class TangibleFactory2 : public swganh::object::ObjectFactory
    {
    public:
        TangibleFactory2(const std::shared_ptr<anh::database::DatabaseManagerInterface>& db_manager);
        void LoadTemplates();

        bool HasTemplate(const std::string& template_name);

        void PersistObject(const std::shared_ptr<swganh::object::Object>& object);

        void DeleteObjectFromStorage(const std::shared_ptr<swganh::object::Object>& object);

        std::shared_ptr<swganh::object::Object> CreateObjectFromStorage(uint64_t object_id);

        std::shared_ptr<swganh::object::Object> CreateObjectFromTemplate(const std::string& template_name);
        
        virtual uint32_t GetType() const;
        const static uint32_t type;
    private:
        std::unordered_map<std::string, std::shared_ptr<swganh::object::tangible::Tangible>>::iterator GetTemplateIter_(const std::string& template_name);
        std::unordered_map<std::string, std::shared_ptr<swganh::object::tangible::Tangible>> tangible_templates_;
    };

}}}  // namespace swganh::object::tangible

#endif  // SWGANH_OBJECT_TANGIBLE_TANGIBLE_FACTORY2_H_
