// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>

#include "swganh/connection/connection_client_interface.h"

namespace swganh_core {
namespace connection {

class ConnectionClient : public swganh::connection::ConnectionClientInterface
{
public:

    ConnectionClient(anh::network::soe::ServerInterface* server, boost::asio::io_service& io_service, boost::asio::ip::udp::endpoint remote_endpoint);
    
    State GetState() const;

    uint32_t GetAccountId() const;

    uint64_t GetPlayerId() const;

    void Connect(uint32_t account_id, uint64_t player_id);
    
    const std::shared_ptr<swganh::object::ObjectController>& GetController() const;

    void SetController(const std::shared_ptr<swganh::object::ObjectController>& controller);

private:

    void OnClose();

   State state_;
    uint32_t account_id_;
    uint64_t player_id_;
    std::shared_ptr<swganh::object::ObjectController> controller_;
};

}}  // namespace swganh::connection
