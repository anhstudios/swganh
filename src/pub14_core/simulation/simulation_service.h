// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef PUB14_CORE_SIMULATION_SIMULATION_SERVICE_H_
#define PUB14_CORE_SIMULATION_SIMULATION_SERVICE_H_

#include "swganh/simulation/simulation_service_interface.h"

namespace swganh_core {
namespace simulation {
    
    class SimulationServiceImpl;

    class SimulationService : public swganh::simulation::SimulationServiceInterface
    {
    public:
        explicit SimulationService(swganh::app::SwganhKernel* kernel);
    
        ~SimulationService();

        anh::service::ServiceDescription GetServiceDescription();

        void StartScene(const std::string& scene_label);
        void StopScene(const std::string& scene_label);

        void RegisterObjectFactories();

        void PersistObject(uint64_t object_id);
		/*
		*	\brief this persists the given object and all related objects (ie: everything contained inside this object)
		*/
		void PersistRelatedObjects(uint64_t parent_object_id);

		void AddObjectToScene(std::shared_ptr<swganh::object::Object> object, const std::string& scene_label);
        
        std::shared_ptr<swganh::object::Object> LoadObjectById(uint64_t object_id);
        std::shared_ptr<swganh::object::Object> LoadObjectById(uint64_t object_id, uint32_t type);
        
        std::shared_ptr<swganh::object::Object> GetObjectById(uint64_t object_id);

		std::shared_ptr<swganh::object::Object> GetObjectByCustomName(const std::wstring& custom_name);
		std::shared_ptr<swganh::object::Object> GetObjectByCustomName(const std::string& custom_name);

		void TransferObjectToScene(uint64_t object_id, const std::string& scene);
		void TransferObjectToScene(uint64_t object_id, const std::string& scene, float x, float y, float z);
		void TransferObjectToScene(std::shared_ptr<swganh::object::Object> object, const std::string& scene);
		void TransferObjectToScene(std::shared_ptr<swganh::object::Object> object, const std::string& scene, float x, float y, float z);
        /**
         * Removes the requested object from the simulation.
         */
        void RemoveObjectById(uint64_t object_id);
        void RemoveObject(const std::shared_ptr<swganh::object::Object>& object);
        
        std::shared_ptr<swganh::object::ObjectController> StartControllingObject(
            const std::shared_ptr<swganh::object::Object>& object,
            std::shared_ptr<swganh::connection::ConnectionClientInterface> client);

        void StopControllingObject(const std::shared_ptr<swganh::object::Object>& object);
        
        void RegisterControllerHandler(uint32_t handler_id, swganh::object::ObjControllerHandler&& handler);

        void UnregisterControllerHandler(uint32_t handler_id);

        void SendToAll(anh::ByteBuffer message);

        void SendToAllInScene(anh::ByteBuffer message, uint32_t scene_id);

        void Startup();

		std::shared_ptr<swganh::object::ObjectManager> GetObjectManager();

    private:

        std::unique_ptr<SimulationServiceImpl> impl_;
		std::shared_ptr<anh::network::soe::ServerInterface> server_;
        swganh::app::SwganhKernel* kernel_;
    };

}}  // namespace swganh_core::simulation

#endif  // PUB14_CORE_SIMULATION_SIMULATION_SERVICE_H_
