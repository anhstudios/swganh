// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_SIMULATION_SIMULATION_SERVICE_H_
#define SWGANH_SIMULATION_SIMULATION_SERVICE_H_

#include <cstdint>
#include <map>
#include <memory>

#include "anh/network/soe/server.h"

#include "swganh/base/base_service.h"
#include "swganh/object/object_controller.h"

namespace anh {
	class ByteBuffer;
}

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

        template<typename MessageType>
        struct GenericControllerHandler
        {
            typedef std::function<void (
                const std::shared_ptr<swganh::object::ObjectController>&, MessageType)
            > HandlerType;
        };
        
        /**
         * Register's a message handler for processing ObjControllerMessage payloads.
         *
         * This overload accepts a member function and a pointer (either naked or smart)
         * and converts the request to the proper message type.
         *
         * \code{.cpp}
         *
         *  RegisterControllerHandler(&MyClass::HandleSomeControllerMessage, this);
         *
         * \param memfunc A member function that can process a concrete ObjControllerMessage type.
         * \param instance An instance of a class that implements memfunc.
         */
        template<typename T, typename U, typename MessageType>
        void RegisterControllerHandler(void (T::*memfunc)(const std::shared_ptr<swganh::object::ObjectController>&, MessageType), U instance)
        {
            RegisterControllerHandler<MessageType>(std::bind(memfunc, instance, std::placeholders::_1, std::placeholders::_2));
        }
        
        /**
         * Register's a message handler for processing ObjControllerMessage payloads.
         *
         * This handler automatically converts the request to the proper message type.
         *
         * \param handler A std::function object representing the handler.
         */
        template<typename MessageType>
        void RegisterControllerHandler(
            typename GenericControllerHandler<MessageType>::HandlerType&& handler)
        {
            auto shared_handler = std::make_shared<typename GenericControllerHandler<MessageType>::HandlerType>(std::move(handler));

            auto wrapped_handler = [this, shared_handler] (
                const std::shared_ptr<swganh::object::ObjectController>& controller,
                swganh::messages::ObjControllerMessage message)
            {
                MessageType tmp(std::move(message));

                (*shared_handler)(controller, std::move(tmp));
            };

            RegisterControllerHandler(MessageType::message_type(), std::move(wrapped_handler));
        }
        
        /**
         * Register's a message handler for processing ObjControllerMessage payloads.
         *
         * This is the low level registration and should be used when wanting to bypass
         * automatic message conversion.
         *
         * \param handler_id The object controller message type.
         * \param handler The object controller handler.
         */
        void RegisterControllerHandler(uint32_t handler_id, swganh::object::ObjControllerHandler&& handler);

        void UnregisterControllerHandler(uint32_t handler_id);

        void SendToAll(anh::ByteBuffer message);

        template <typename T>
        void SendToAll(const T& message)
        {
            anh::ByteBuffer message_buffer;
            message.Serialize(message_buffer);

            SendToAll(message_buffer);
        }

        void SendToAllInScene(anh::ByteBuffer message, uint32_t scene_id);

        template<typename T>
        void SendToAllInScene(const T& message, uint32_t scene_id)
        {
            anh::ByteBuffer message_buffer;
            message.Serialize(message_buffer);

            SendToAllInScene(message_buffer, scene_id);
        }

    private:

        void onStart();

        std::unique_ptr<SimulationServiceImpl> impl_;
		std::shared_ptr<anh::network::soe::ServerInterface> server_;
    };

}}  // namespace swganh::simulation

#endif  // SWGANH_SIMULATION_SIMULATION_SERVICE_H_
