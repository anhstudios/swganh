
#ifndef SWGANH_OBJECT_OBJECT_FACTORY_INTERFACE_H_
#define SWGANH_OBJECT_OBJECT_FACTORY_INTERFACE_H_

namespace swganh {
namespace object {

    class ObjectFactoryInterface
    {
    public:
        virtual ~ObjectFactoryInterface() {}
        
        /**
         * Loads templates from storage into memory. Templates are preconfigured
         * object instances with no ID assigned. These templates are used as
         * prototypes in the construction of new objects.
         */
        virtual void LoadTemplates() = 0;

        /**
         * @returns true if the requested template exists in this factory, false if not.
         */
        virtual bool HasTemplate(const std::string& template_name) = 0;

        /**
         * Persists the object's state to storage.
         *
         * @param object the object instance to persist.
         */
        virtual void PersistObject(const std::shared_ptr<BaseObject>& object) = 0;

        /**
         * Deletes the requested object from storage.
         *
         * @param object the object instance to delete from storage.
         */
        virtual void DeleteObjectFromStorage(const std::shared_ptr<BaseObject>& object) = 0;

        /**
         * Creates an instance of a stored object with the specified id.
         *
         * @returns the created object instance.
         * @throws InvalidObject when no object exists for the specified id.
         */
        virtual std::shared_ptr<BaseObject> CreateObjectFromStorage(uint64_t object_id) = 0;
                
        /**
         * Creates an instance of an object from the specified template.
         *
         * @returns the created object instance.
         * @throws InvalidObjectTemplate when the specified template does not exist.
         */
        virtual std::shared_ptr<BaseObject> CreateObjectFromTemplate(const std::string& template_name) = 0;
    };

}}  // namespace swganh::object

#endif  // SWGANH_OBJECT_OBJECT_FACTORY_INTERFACE_H_
