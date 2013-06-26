// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "connection_client.h"

#include "swganh/observer/observer_interface.h"
#include "swganh/observer/observer_interface.h"
#include "swganh_core/object/object.h"
#include "swganh_core/object/player/player.h"

using namespace swganh::network;
using namespace std;
using namespace swganh::connection;
using namespace swganh::object;
using namespace swganh::observer;

ConnectionClient::ConnectionClient(ServerInterface* server, boost::asio::io_service& cpu_pool, boost::asio::ip::udp::endpoint remote_endpoint)
    : ConnectionClientInterface(server, cpu_pool, remote_endpoint)
{}

ConnectionClient::State ConnectionClient::GetState() const
{
    return state_;
}

uint32_t ConnectionClient::GetAccountId() const
{
    return account_id_;
}

uint64_t ConnectionClient::GetPlayerId() const
{
    return player_id_;
}

void ConnectionClient::Connect(uint32_t account_id, uint64_t player_id)
{
    account_id_ = account_id;
    player_id_ = player_id;

    state_ = CONNECTED;
}

void ConnectionClient::OnClose()
{
    state_ = DISCONNECTING;
}

const shared_ptr<ObserverInterface>& ConnectionClient::GetController() const
{
    return controller_;
}

void ConnectionClient::SetController(const shared_ptr<ObserverInterface>& controller)
{
    controller_ = controller;
}
