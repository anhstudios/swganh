
#ifndef SWGANH_CONNECTION_BASE_CONNECTION_SERVICE_H_
#define SWGANH_CONNECTION_BASE_CONNECTION_SERVICE_H_

#include <cstdint>
#include <memory>
#include <string>

#include <tbb/concurrent_hash_map.h>

#include "anh/hash_string.h"

#include "anh/network/soe/packet.h"
#include "anh/network/soe/packet_utilities.h"
#include "anh/network/soe/session.h"

#include "swganh/base/base_service.h"
#include "swganh/base/swg_message_router.h"

#include "swganh/character/base_character_service.h"
#include "swganh/login/base_login_service.h"

namespace anh {
namespace network {
namespace soe {
class Server;
}}}  // namespace anh::network::soe

namespace swganh {
namespace connection {
    
struct ConnectionClient;
    
class BaseConnectionService 
    : public swganh::base::BaseService
    , public swganh::base::SwgMessageRouter<ConnectionClient>
{
public:
    typedef tbb::concurrent_hash_map<
        boost::asio::ip::udp::endpoint, 
        std::shared_ptr<ConnectionClient>,
        anh::network::soe::EndpointHashCompare
    > ClientMap;
public:
    BaseConnectionService(
        std::string listen_address, 
        uint16_t listen_port, 
        std::shared_ptr<anh::app::KernelInterface> kernel);
    
    void DescribeConfigOptions(boost::program_options::options_description& description);

    void onStart();
    void onStop();
    
    std::shared_ptr<ConnectionClient> GetClientFromEndpoint(
        const boost::asio::ip::udp::endpoint& remote_endpoint);

protected:
    virtual void OnDescribeConfigOptions(boost::program_options::options_description& description) = 0;
    
    const std::string& listen_address();

    uint16_t listen_port();

    ClientMap& clients();

    std::unique_ptr<anh::network::soe::Server>& server();
    
    std::shared_ptr<swganh::character::BaseCharacterService> character_service();

    std::shared_ptr<swganh::login::BaseLoginService> login_service();

private:    
    ClientMap clients_;
    
    std::unique_ptr<anh::network::soe::Server> soe_server_;
    
    std::shared_ptr<swganh::character::BaseCharacterService> character_service_;
    std::shared_ptr<swganh::login::BaseLoginService> login_service_;

    std::string listen_address_;
    uint16_t listen_port_;
};
    
}}  // namespace swganh::connection

#endif  // SWGANH_CONNECTION_BASE_CONNECTION_SERVICE_H_
