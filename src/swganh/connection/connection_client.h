
#ifndef SWGANH_CONNECTION_CONNECTION_CLIENT_H_
#define SWGANH_CONNECTION_CONNECTION_CLIENT_H_

#include <cstdint>
#include <memory>

namespace anh {
namespace network {
namespace soe {
class Session;
}}}  // namespace anh::network::soe

namespace swganh {
namespace connection {

struct ConnectionClient {
    uint32_t account_id;
    uint64_t player_id;
    std::shared_ptr<anh::network::soe::Session> session;
};

}}  // namespace swganh::connection

#endif  // SWGANH_CONNECTION_CONNECTION_CLIENT_H_
