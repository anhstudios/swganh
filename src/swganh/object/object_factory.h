
#ifndef SWGANH_OBJECT_OBJECT_FACTORY_H_
#define SWGANH_OBJECT_OBJECT_FACTORY_H_

#include "swganh/object/object_factory_interface.h"

namespace anh {
namespace database {
class DatabaseManagerInterface;
}} // anh::database

namespace sql {
class ResultSet;
}

namespace swganh {
namespace object {

    class Object;

    class ObjectFactory : public ObjectFactoryInterface
    {
    public:
        ObjectFactory(const std::shared_ptr<anh::database::DatabaseManagerInterface>& db_manager);
        virtual ~ObjectFactory() {}
        
        void CreateBaseObjectFromStorage(const std::shared_ptr<Object>& object, const std::shared_ptr<sql::ResultSet>& result);

    protected:
        std::shared_ptr<anh::database::DatabaseManagerInterface> db_manager_;   
    };

}}  // namespace swganh::object

#endif  // SWGANH_OBJECT_OBJECT_FACTORY_H_
