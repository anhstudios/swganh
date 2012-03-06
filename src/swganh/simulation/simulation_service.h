
#ifndef SWGANH_SIMULATION_SIMULATION_SERVICE_H_
#define SWGANH_SIMULATION_SIMULATION_SERVICE_H_

#include <cstdint>
#include <map>
#include <memory>

#include "swganh/base/base_service.h"
#include "swganh/object/object_controller.h"

namespace swganh {
namespace connection {
    class ConnectionClient;
}}  // namespace swganh::network

namespace swganh {
namespace object {
    class Object;
}}  // namespace swganh::object

namespace swganh {
namespace simulation {
    
    class SimulationServiceImpl;

    class SimulationService : public swganh::base::BaseService
    {
    public:
        explicit SimulationService(anh::app::KernelInterface* kernel);
    
        ~SimulationService();

        anh::service::ServiceDescription GetServiceDescription();

        void StartScene(const std::string& scene_label);
        void StopScene(const std::string& scene_label);

        void RegisterObjectFactories(anh::app::KernelInterface* kernel);

        void PersistObject(uint64_t object_id);
		/*
		*	\brief this persists the given object and all related objects (ie: everything contained inside this object)
		*/
		void PersistRelatedObjects(uint64_t parent_object_id);
        
        std::shared_ptr<swganh::object::Object> LoadObjectById(uint64_t object_id);
        std::shared_ptr<swganh::object::Object> LoadObjectById(uint64_t object_id, uint32_t type);
        
        template<typename T>
        std::shared_ptr<T> LoadObjectById(uint64_t object_id)
        {
            std::shared_ptr<swganh::object::Object> object = LoadObjectById(object_id, T::type);
#ifdef _DEBUG
            return std::dynamic_pointer_cast<T>(object);
#else
            return std::static_pointer_cast<T>(object);
#endif
        }
        
        std::shared_ptr<swganh::object::Object> GetObjectById(uint64_t object_id);

        template<typename T>
        std::shared_ptr<T> GetObjectById(uint64_t object_id)
        {
            std::shared_ptr<swganh::object::Object> object = GetObjectById(object_id);

#ifdef _DEBUG
            return std::dynamic_pointer_cast<T>(move(object));
#else
            return std::static_pointer_cast<T>(move(object));
#endif
        }

        /**
         * Removes the requested object from the simulation.
         */
        void RemoveObjectById(uint64_t object_id);
        void RemoveObject(const std::shared_ptr<swganh::object::Object>& object);
        
        std::shared_ptr<swganh::object::ObjectController> StartControllingObject(
            const std::shared_ptr<swganh::object::Object>& object,
            std::shared_ptr<swganh::connection::ConnectionClient> client);

        void StopControllingObject(const std::shared_ptr<swganh::object::Object>& object);

        void RegisterControllerHandler(uint32_t handler_id, swganh::object::ObjControllerHandler&& handler);

        void UnregisterControllerHandler(uint32_t handler_id);

    private:

        void onStart();

        std::unique_ptr<SimulationServiceImpl> impl_;
    };

}}  // namespace swganh::simulation

#endif  // SWGANH_SIMULATION_SIMULATION_SERVICE_H_
