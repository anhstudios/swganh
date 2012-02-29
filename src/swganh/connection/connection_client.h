
#ifndef SWGANH_CONNECTION_CONNECTION_CLIENT_H_
#define SWGANH_CONNECTION_CONNECTION_CLIENT_H_

#include <cstdint>

#include "anh/network/soe/session.h"

namespace swganh {
namespace object {
    class ObjectController;
}}  // namespace swganh::object

namespace swganh {
namespace connection {

class ConnectionClient : public anh::network::soe::Session
{
public:
    enum State
    {
        CONNECTING = 0,
        CONNECTED,
        PLAYING,
        DISCONNECTING
    };
    
    ConnectionClient(boost::asio::ip::udp::endpoint remote_endpoint, anh::network::soe::ServerInterface* server);
    
    State GetState() const;

    uint32_t GetAccountId() const;

    uint64_t GetPlayerId() const;

    void Connect(uint32_t account_id, uint64_t player_id);

    void Disconnect();
    
    const std::shared_ptr<swganh::object::ObjectController>& GetController() const;

    void SetController(const std::shared_ptr<swganh::object::ObjectController>& controller);

private:
    State state_;
    uint32_t account_id_;
    uint64_t player_id_;
    std::shared_ptr<swganh::object::ObjectController> controller_;
};

}}  // namespace swganh::connection

#endif  // SWGANH_CONNECTION_CONNECTION_CLIENT_H_
