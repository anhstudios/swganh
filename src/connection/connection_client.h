
#ifndef CONNECTION_CONNECTION_CLIENT_H_
#define CONNECTION_CONNECTION_CLIENT_H_

#include <cstdint>
#include <memory>

namespace anh {
namespace network {
namespace soe {
class Session;
}}}  // namespace anh::network::soe

namespace connection {

struct ConnectionClient {
    uint32_t account_id;
    uint64_t player_id;
    std::shared_ptr<anh::network::soe::Session> session;
};

}  // namespace connection

#endif  // CONNECTION_CONNECTION_CLIENT_H_
