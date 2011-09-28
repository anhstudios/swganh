
#ifndef SWGANH_CONNECTION_CONNECTION_CLIENT_H_
#define SWGANH_CONNECTION_CONNECTION_CLIENT_H_

#include <cstdint>

#include "swganh/network/remote_client.h"

namespace swganh {
namespace object {
    class ObjectController;
}}  // namespace swganh::object

namespace swganh {
namespace connection {

class ConnectionClient : public swganh::network::RemoteClient 
{
public:
    enum State
    {
        CONNECTING = 0,
        CONNECTED,
        PLAYING,
        DISCONNECTING
    };
    
    explicit ConnectionClient(
        std::shared_ptr<anh::network::soe::Session> session);

    ~ConnectionClient();

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
