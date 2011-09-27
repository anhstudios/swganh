
#include "swganh/connection/connection_client.h"

using namespace anh::network::soe;
using namespace std;
using namespace swganh::connection;

ConnectionClient::ConnectionClient(
    shared_ptr<Session> session)
    : RemoteClient(session)
    , state_(CONNECTING)
{}

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

void ConnectionClient::Disconnect()
{
    state_ = DISCONNECTING;
    GetSession()->Close();
}
