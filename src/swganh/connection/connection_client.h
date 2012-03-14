
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

#endif  // SWGANH_CONNECTION_CONNECTION_CLIENT_H_
