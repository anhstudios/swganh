// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <map>
#include <memory>

#include "swganh/network/soe/server.h"
#include "swganh/service/service_interface.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh/object/object_controller_interface.h"
#include "swganh/object/permissions/permission_type.h"

namespace swganh {
	class ByteBuffer;
}

namespace swganh {
namespace connection {
    class ConnectionClient;
}}  // namespace swganh::network

namespace swganh {
namespace object {

	typedef std::function<
        void (std::shared_ptr<swganh::object::Object>, swganh::messages::ObjControllerMessage*)
    > ObjControllerHandler;

    typedef Concurrency::concurrent_unordered_map<
        uint32_t, 
        ObjControllerHandler
    > ObjControllerHandlerMap;

    class Object;
	class ObjectManager;
}}  // namespace swganh::object

namespace swganh {
namespace simulation {
    
    class SimulationServiceInterface : public swganh::service::ServiceInterface
    {
    public:
        virtual void StartScene(const std::string& scene_label) = 0;
        virtual void StopScene(const std::string& scene_label) = 0;

		virtual void AddObjectToScene(std::shared_ptr<swganh::object::Object> object, const std::string& scene_label) = 0;

        virtual void RegisterObjectFactories() = 0;

        virtual void PersistObject(uint64_t object_id) = 0;
		/*
		*	\brief this persists the given object and all related objects (ie: everything contained inside this object)
		*/
		virtual void PersistRelatedObjects(uint64_t parent_object_id) = 0;
        
        virtual std::shared_ptr<swganh::object::Object> LoadObjectById(uint64_t object_id) = 0;
        virtual std::shared_ptr<swganh::object::Object> LoadObjectById(uint64_t object_id, uint32_t type) = 0;

		virtual std::shared_ptr<swganh::object::Object> GetObjectByCustomName(const std::wstring& custom_name) = 0;
		virtual std::shared_ptr<swganh::object::Object> GetObjectByCustomName(const std::string& custom_name) = 0;

		virtual void TransferObjectToScene(uint64_t object_id, const std::string& scene) = 0;
		virtual void TransferObjectToScene(uint64_t object_id, const std::string& scene, float x, float y, float z) = 0;
		virtual void TransferObjectToScene(std::shared_ptr<swganh::object::Object> object, const std::string& scene) = 0;
		virtual void TransferObjectToScene(std::shared_ptr<swganh::object::Object> object, const std::string& scene, float x, float y, float z) = 0;
        
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
        
        virtual std::shared_ptr<swganh::object::Object> GetObjectById(uint64_t object_id) = 0;

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
        virtual void RemoveObjectById(uint64_t object_id) = 0;
        virtual void RemoveObject(const std::shared_ptr<swganh::object::Object>& object) = 0;
        
        virtual std::shared_ptr<swganh::observer::ObserverInterface> StartControllingObject(
            const std::shared_ptr<swganh::object::Object>& object,
            std::shared_ptr<swganh::connection::ConnectionClientInterface> client) = 0;

        virtual void StopControllingObject(const std::shared_ptr<swganh::object::Object>& object) = 0;

        template<typename MessageType>
        struct GenericControllerHandler
        {
            typedef std::function<void (
                std::shared_ptr<swganh::object::Object>, MessageType*)
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
        void RegisterControllerHandler(void (T::*memfunc)(const std::shared_ptr<swganh::object::Object>&, MessageType*), U instance)
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
                std::shared_ptr<swganh::object::Object> object,
                swganh::messages::ObjControllerMessage* message)
            {
                MessageType tmp(*message);
				tmp.OnControllerDeserialize(message->data);

				(*shared_handler)(object, &tmp);
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
        virtual void RegisterControllerHandler(uint32_t handler_id, swganh::object::ObjControllerHandler&& handler) = 0;

        virtual void UnregisterControllerHandler(uint32_t handler_id) = 0;

        virtual void SendToAll(swganh::ByteBuffer message) = 0;

        template <typename T>
        void SendToAll(const T& message)
        {
            swganh::ByteBuffer message_buffer;
            message.Serialize(message_buffer);

            SendToAll(message_buffer);
        }

        virtual void SendToAllInScene(swganh::ByteBuffer message, uint32_t scene_id) = 0;

        template<typename T>
        void SendToAllInScene(const T& message, uint32_t scene_id)
        {
            swganh::ByteBuffer message_buffer;
            message.Serialize(message_buffer);

            SendToAllInScene(message_buffer, scene_id);
        }

		virtual std::shared_ptr<swganh::object::Object> CreateObjectFromTemplate(const std::string& template_name, 
			swganh::object::PermissionType type=swganh::object::DEFAULT_PERMISSION, bool is_persisted=true, bool is_initialized=true, uint64_t object_id=0) = 0;

		virtual void PrepareToAccomodate(uint32_t delta) = 0;
    };

}}  // namespace swganh::simulation
