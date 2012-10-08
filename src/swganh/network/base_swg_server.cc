// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "base_swg_server.h"

#include "swganh/logger.h"
#include "swganh/network/soe/session.h"

using namespace swganh::network;

using std::move;

BaseSwgServer::BaseSwgServer(
    boost::asio::io_service& io_service)
    : swganh::network::soe::Server(io_service)
{}

void BaseSwgServer::HandleMessage(
    const std::shared_ptr<swganh::network::soe::Session>& connection,
    swganh::ByteBuffer message)
{
    uint32_t message_type = message.peekAt<uint32_t>(message.read_position() + sizeof(uint16_t));

    auto find_iter = message_handlers_.find(message_type);
    if (find_iter == message_handlers_.end())
    {
        DLOG(warning) << "Received an unidentified message: " << std::hex << message_type;
        return;
    }

    try
    {
        LOG_NET << "HandleMessage: "  << std::hex << message_type << " Client -> Server \n" << message;
        find_iter->second(connection, move(message));
    }
    catch(std::exception& e)
    {
        LOG(error) << e.what();
    }
}

void BaseSwgServer::RegisterMessageHandler(
    swganh::HashString handler_id,
    SwgMessageHandler&& handler)
{
    if (HasHandler(handler_id))
    {
        throw HandlerAlreadyDefined("Requested registration of handler that has already been defined.");
    }

    message_handlers_.insert(make_pair(handler_id, move(handler)));
}

bool BaseSwgServer::HasHandler(swganh::HashString handler_id)
{
    return (message_handlers_.find(handler_id) != message_handlers_.end());
}
