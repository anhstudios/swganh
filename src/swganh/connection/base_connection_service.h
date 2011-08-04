
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

#include "swganh/character/base_character_service.h"
#include "swganh/login/login_service_interface.h"

namespace anh {
namespace network {
namespace soe {
class Server;
}}}  // namespace anh::network::soe

namespace swganh {
namespace connection {
    
struct ConnectionClient;
    
class BaseConnectionService : public swganh::base::BaseService
{
public:
    typedef std::function<void (
        std::shared_ptr<ConnectionClient> client, 
        std::shared_ptr<anh::network::soe::Packet> packet)
    > MessageHandler;

    typedef tbb::concurrent_hash_map<
        boost::asio::ip::udp::endpoint, 
        std::shared_ptr<ConnectionClient>,
        anh::network::soe::EndpointHashCompare
    > ClientMap;

    typedef tbb::concurrent_hash_map<
        anh::HashString,
        std::pair<MessageHandler, bool> // bool IsClientRequired
    > MessageHandlerMap;

public:
    explicit BaseConnectionService(std::shared_ptr<anh::app::KernelInterface> kernel);
    
    void DescribeConfigOptions(boost::program_options::options_description& description);

    void onStart();
    void onStop();

    /**
     * Register a handler with the connection service to be invoked whenever a message of the
     * specified type is received. Throws an exception if a handler already exists.
     *
     * By default if a message is received from a source with no exising client registration the
     * handler will throw a runtime_error. However, if the client_required flag is set to false
     * then a new client object is created and given the session that generated the message.
     *
     * @param handler The handler to register with the connection service.
     * @param client_required Defaults to true and should be the common case for nearly ever 
     *                        message handler.
     *
     * @throws std::runtime_exception
     */
    template<typename MessageType>
    void RegisterMessageHandler(
        std::function<void (std::shared_ptr<ConnectionClient>, MessageType)> handler,
        bool client_required = true)
    {
        MessageHandlerMap::accessor a;
        if (handlers_.find(a, MessageType::opcode)) {
            DLOG(WARNING) << "Handler has already been defined: " << std::hex << MessageType::opcode;
            throw std::runtime_error("Requested registration of handler that has already been defined.");
        }

        auto wrapped_handler = [this, handler] (
            std::shared_ptr<ConnectionClient> client, 
            std::shared_ptr<anh::network::soe::Packet> packet) 
        {                        
            MessageType message;
            message.deserialize(*packet->message());        
            
            handler(client, message);
        };

        handlers_.insert(std::make_pair(MessageType::opcode, std::make_pair(wrapped_handler, client_required)));
    }
    
    void HandleMessage(std::shared_ptr<anh::network::soe::Packet> packet);
    
    std::shared_ptr<ConnectionClient> GetClientFromEndpoint(
        const boost::asio::ip::udp::endpoint& remote_endpoint);

protected:
    virtual void OnDescribeConfigOptions(boost::program_options::options_description& description) = 0;
    
    const std::string& listen_address();

    uint16_t listen_port();

    ClientMap& clients();

    std::unique_ptr<anh::network::soe::Server>& server();
    
    std::shared_ptr<swganh::character::BaseCharacterService> character_service();

    std::shared_ptr<swganh::login::LoginServiceInterface> login_service();

private:    
    ClientMap clients_;
    MessageHandlerMap handlers_;
    
    std::unique_ptr<anh::network::soe::Server> soe_server_;
    
    std::shared_ptr<swganh::character::BaseCharacterService> character_service_;
    std::shared_ptr<swganh::login::LoginServiceInterface> login_service_;

    std::string listen_address_;
    uint16_t listen_port_;
};
    
}}  // namespace swganh::connection

#endif  // SWGANH_CONNECTION_BASE_CONNECTION_SERVICE_H_
