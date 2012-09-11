// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh/simulation/simulation_service_interface.h"

namespace swganh {
namespace simulation {
    
    class SimulationServiceImpl;

    class SimulationService : public swganh::simulation::SimulationServiceInterface
    {
    public:
        explicit SimulationService(swganh::app::SwganhKernel* kernel);
    
        ~SimulationService();

        swganh::service::ServiceDescription GetServiceDescription();

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
        
        std::shared_ptr<swganh::observer::ObserverInterface> StartControllingObject(
            const std::shared_ptr<swganh::object::Object>& object,
            std::shared_ptr<swganh::connection::ConnectionClientInterface> client);

        void StopControllingObject(const std::shared_ptr<swganh::object::Object>& object);
        
        void RegisterControllerHandler(uint32_t handler_id, swganh::object::ObjControllerHandler&& handler);

        void UnregisterControllerHandler(uint32_t handler_id);

        void SendToAll(swganh::ByteBuffer message);

        void SendToAllInScene(swganh::ByteBuffer message, uint32_t scene_id);

        void Startup();

        virtual std::shared_ptr<swganh::object::Object> CreateObjectFromTemplate(const std::string& template_name, 
			swganh::object::PermissionType type=swganh::object::DEFAULT_PERMISSION, bool is_persisted=true, 
			bool is_initialized=true, uint64_t object_id=0);
		
		virtual void PrepareToAccomodate(uint32_t delta);

    private:

        std::unique_ptr<SimulationServiceImpl> impl_;
		std::shared_ptr<swganh::network::soe::ServerInterface> server_;
        swganh::app::SwganhKernel* kernel_;
    };

}}  // namespace swganh::simulation
