// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_NETWORK_BASE_SWG_SERVER_H_
#define SWGANH_NETWORK_BASE_SWG_SERVER_H_

#include <functional>
#include <map>
#include <memory>
#include <mutex>

#include <concurrent_unordered_map.h>

#include "anh/hash_string.h"
#include "anh/network/soe/server.h"

namespace anh {

    class ByteBuffer;

}  // namespace anh::network::soe


namespace swganh {
namespace network {
    
    class BaseSwgServer : public anh::network::soe::Server
    {
    public:
        
        template<typename MessageType>
        struct GenericMessageHandler
        {
            typedef std::function<void (
                const std::shared_ptr<anh::network::soe::Session>&, MessageType)
            > Type;
        };

        typedef std::function<void (
            std::shared_ptr<anh::network::soe::Session>, 
            std::shared_ptr<anh::ByteBuffer> message)
        > SwgMessageHandler;

        typedef std::runtime_error HandlerAlreadyDefined;
        typedef std::runtime_error UnidentifiedMessageReceived;
        typedef std::runtime_error ValidClientRequired;
        
        void HandleMessage(
            std::shared_ptr<anh::network::soe::Session> connection, 
            std::shared_ptr<anh::ByteBuffer> message);

        template<typename MessageType>
        void RegisterMessageHandler(
            typename GenericMessageHandler<MessageType>::Type&& handler)
        {
            auto shared_handler = make_shared<typename GenericMessageHandler<MessageType>::Type>(move(handler));

            auto wrapped_handler = [this, shared_handler] (
                std::shared_ptr<anh::network::soe::Session> client, 
                std::shared_ptr<anh::ByteBuffer> message)
            {
                MessageType tmp;
                tmp.deserialize(*message);

                (*shared_handler)(client, std::move(tmp));
            };

            RegisterMessageHandler(MessageType::opcode(), move(wrapped_handler));
        }

        void RegisterMessageHandler(
            anh::HashString handler_id, 
            SwgMessageHandler&& handler);

        bool HasHandler(anh::HashString handler_id);
        
    private:
        typedef Concurrency::concurrent_unordered_map<
            anh::HashString,
            SwgMessageHandler
        > MessageHandlerMap;
        
        MessageHandlerMap message_handlers_;
    };
    
}}  // namespace swganh::network

#endif  // SWGANH_NETWORK_BASE_SWG_SERVER_H_
