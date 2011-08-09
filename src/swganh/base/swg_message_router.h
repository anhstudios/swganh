/*
 This file is part of MMOServer. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2010 The SWG:ANH Team

 MMOServer is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 MMOServer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with MMOServer.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SWGANH_BASE_SWG_MESSAGE_ROUTER_H_
#define SWGANH_BASE_SWG_MESSAGE_ROUTER_H_

#include <exception>
#include <functional>
#include <memory>

#include <boost/asio/ip/udp.hpp>
#include <glog/logging.h>
#include <tbb/concurrent_hash_map.h>

#include "anh/byte_buffer.h"
#include "anh/hash_string.h"
#include "anh/network/soe/packet.h"
#include "anh/network/soe/session.h"

namespace swganh {
namespace base {
      
template<typename ClientType>
class SwgMessageRouter {
public:
    typedef std::function<std::shared_ptr<ClientType> (
        const boost::asio::ip::udp::endpoint&)
    > ClientLookup;

    typedef std::function<void (
        std::shared_ptr<ClientType> client, 
        std::shared_ptr<anh::network::soe::Packet> packet)
    > MessageHandler;

    typedef tbb::concurrent_hash_map<
        anh::HashString,
        std::pair<MessageHandler, bool> // bool IsClientRequired
    > MessageHandlerMap;

    typedef std::runtime_error HandlerAlreadyDefined;
    typedef std::runtime_error UnidentifiedMessageReceived;
    typedef std::runtime_error ValidClientRequired;

    explicit SwgMessageRouter(ClientLookup&& lookup)
        : find_client_(move(lookup)) {}

    /**
     * Register a handler with the connection service to be invoked whenever a message of the
     * specified type is received. Throws an exception if a handler already exists.
     *
     * By default if a message is received from a source with no exising client registration the
     * handler will throw a runtime_error. However, if the client_required flag is set to false
     * then a new client object is created and given the session that generated the message.
     *
     * @param handler The handler to register with the connection service.
     * @param client_required Defaults to true and should be the common case for nearly ever 
     *                        message handler.
     *
     * @throws HandlerAlreadyDefined
     */
    template<typename MessageType>
    void RegisterMessageHandler(
        std::function<void (std::shared_ptr<ClientType>, MessageType)> handler,
        bool client_required = true)
    {
        typename MessageHandlerMap::accessor a;
        if (handlers_.find(a, MessageType::opcode())) {
            DLOG(WARNING) << "Handler has already been defined: " << std::hex << MessageType::opcode();
            throw HandlerAlreadyDefined("Requested registration of handler that has already been defined.");
        }

        auto wrapped_handler = [this, handler] (
            std::shared_ptr<ClientType> client, 
            std::shared_ptr<anh::network::soe::Packet> packet) 
        {                        
            MessageType message;
            message.deserialize(*packet->message());        
            
            handler(client, message);
        };

        handlers_.insert(std::make_pair(MessageType::opcode(), std::make_pair(wrapped_handler, client_required)));
    }

    /**
     * Routes a message using the registered message handlers.
     *
     * @param packet The packet to route.
     *
     * @throws UnidentifiedMessageReceived
     * @throws ValidClientRequired
     */
    void RouteMessage(std::shared_ptr<anh::network::soe::Packet> packet) {
        auto message = packet->message();

        uint32_t message_type = message->peekAt<uint32_t>(message->read_position() + sizeof(uint16_t));
        
        typename MessageHandlerMap::accessor a;

        // No handler specified, trigger an event.
        if (!handlers_.find(a, message_type)) {
            DLOG(WARNING) << "Received message with no handler, triggering event: "
                    << std::hex << message_type << "\n\n" << *message; 
            
            throw UnidentifiedMessageReceived("Received an unidentified message");

            return;
        }
        
        auto client = find_client_(packet->session()->remote_endpoint());

        if (!client) {
            if (a->second.second) {
                DLOG(WARNING) << "Received a message from an invalid source: "
                        << std::hex << message_type << "\n\n" << *message; 

                throw ValidClientRequired("A valid client is required to invoke this message handler");
            } else {
                client = std::make_shared<ClientType>();
                client->session = packet->session();
            }
        }

        a->second.first(client, packet);
    }

private:    
    MessageHandlerMap handlers_;
    ClientLookup find_client_;
};
        
}}  // namespace swganh::base

#endif  // SWGANH_BASE_SWG_MESSAGE_ROUTER_H_
