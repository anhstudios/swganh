
#include "swganh/connection/connection_client.h"
#include "swganh/object/object_controller.h"
#include "swganh/object/object.h"
#include "swganh/object/player/player.h"

using namespace anh::network::soe;
using namespace std;
using namespace swganh::connection;
using namespace swganh::object;

ConnectionClient::ConnectionClient(
    boost::asio::ip::udp::endpoint remote_endpoint, ServerInterface* server)
    : Session(remote_endpoint, server)
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

    // @TODO REFACTOR Move this functionality out to a PlayerService
    auto object = controller_->GetObject();
    auto contained = object->GetContainedObjects();
    
    for_each(
        begin(contained),
        end(contained),
        [] (Object::ObjectMap::value_type& object_entry)
    {
        auto player = dynamic_pointer_cast<player::Player>(object_entry.second);

        if (player)
        {
            player->AddStatusFlag(player::LD);
        }
    });
    // END TODO
}

const shared_ptr<ObjectController>& ConnectionClient::GetController() const
{
    return controller_;
}

void ConnectionClient::SetController(const shared_ptr<ObjectController>& controller)
{
    controller_ = controller;
}
