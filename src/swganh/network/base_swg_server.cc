// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "base_swg_server.h"

#include "anh/byte_buffer.h"

#include "anh/network/soe/session.h"

using namespace swganh::network;
        
void BaseSwgServer::HandleMessage(
    std::shared_ptr<anh::network::soe::Session> connection, 
    std::shared_ptr<anh::ByteBuffer> message)
{
    uint32_t message_type = message->peekAt<uint32_t>(message->read_position() + sizeof(uint16_t));

    auto find_iter = message_handlers_.find(message_type);
    if (find_iter == message_handlers_.end())
    {
        throw UnidentifiedMessageReceived("Received an unidentified message");
    }

    find_iter->second(connection, message);
}

void BaseSwgServer::RegisterMessageHandler(
    anh::HashString handler_id, 
    SwgMessageHandler&& handler)
{
    if (HasHandler(handler_id))
    {
        throw HandlerAlreadyDefined("Requested registration of handler that has already been defined.");
    }

    message_handlers_.insert(make_pair(handler_id, move(handler)));
}

bool BaseSwgServer::HasHandler(anh::HashString handler_id)
{
    return (message_handlers_.find(handler_id) != message_handlers_.end());
}
