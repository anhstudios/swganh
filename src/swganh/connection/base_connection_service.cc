
#include "swganh/connection/base_connection_service.h"

#include <boost/lexical_cast.hpp>
#include <glog/logging.h>

#include "anh/crc.h"
#include "anh/event_dispatcher/basic_event.h"
#include "anh/event_dispatcher/event_dispatcher_interface.h"
#include "anh/network/soe/packet.h"
#include "anh/network/soe/server.h"
#include "anh/service/service_manager.h"

#include "swganh/app/swganh_kernel.h"

#include "swganh/connection/connection_client.h"

using namespace anh::network;

using namespace swganh::base;
using namespace swganh::character;
using namespace swganh::connection;
using namespace swganh::login;

using namespace std;

using boost::asio::ip::udp;

BaseConnectionService::BaseConnectionService(std::shared_ptr<anh::app::KernelInterface> kernel)
    : BaseService(kernel)
    , soe_server_(nullptr)
{
    soe_server_.reset(new soe::Server(
        kernel->GetIoService(),
        bind(&BaseConnectionService::HandleMessage, this, placeholders::_1)));
    soe_server_->event_dispatcher(kernel->GetEventDispatcher());
}

void BaseConnectionService::DescribeConfigOptions(boost::program_options::options_description& description) {
    description.add_options()
        ("service.connection.udp_port", boost::program_options::value<uint16_t>(&listen_port_),
            "The port the connection service will listen for incoming client connections on")
        ("service.connection.address", boost::program_options::value<string>(&listen_address_),
            "The public address the connection service will listen for incoming client connections on")
    ;

    OnDescribeConfigOptions(description);
}

void BaseConnectionService::onStart() {
    soe_server_->Start(listen_port_);
    
    character_service_ = std::static_pointer_cast<BaseCharacterService>(kernel()->GetServiceManager()->GetService("CharacterService"));    
    login_service_ = std::static_pointer_cast<swganh::login::LoginServiceInterface>(kernel()->GetServiceManager()->GetService("LoginService"));
}

void BaseConnectionService::onStop() {
    soe_server_->Shutdown();
    clients_.clear();
}

const string& BaseConnectionService::listen_address() {
    return listen_address_;
}

uint16_t BaseConnectionService::listen_port() {
    return listen_port_;
}

BaseConnectionService::ClientMap& BaseConnectionService::clients() {
    return clients_;
}

std::unique_ptr<anh::network::soe::Server>& BaseConnectionService::server() {
    return soe_server_;
}

shared_ptr<BaseCharacterService> BaseConnectionService::character_service() {
    return character_service_;
}

shared_ptr<LoginServiceInterface> BaseConnectionService::login_service() {
    return login_service_;
}

void BaseConnectionService::HandleMessage(shared_ptr<soe::Packet> packet) 
{
    auto message = packet->message();

    uint32_t message_type = message->peekAt<uint32_t>(message->read_position() + sizeof(uint16_t));
    
    MessageHandlerMap::accessor a;

    // No handler specified, trigger an event.
    if (!handlers_.find(a, message_type)) {
        DLOG(WARNING) << "Received message with no handler, triggering event: "
                << std::hex << message_type << "\n\n" << *message; 

        kernel()->GetEventDispatcher()->trigger(
            anh::event_dispatcher::make_shared_event(
                message_type,
                *packet));

        return;
    }
    
    auto client = GetClientFromEndpoint(packet->session()->remote_endpoint());

    if (!client) {
        if (a->second.second) {
            DLOG(WARNING) << "Received a message from an invalid source: "
                    << std::hex << message_type << "\n\n" << *message; 

            throw std::runtime_error("A valid client is required to invoke this message handler");
        } else {
            client = make_shared<ConnectionClient>();
            client->session = packet->session();
        }
    }

    a->second.first(client, packet);
}

shared_ptr<ConnectionClient> BaseConnectionService::GetClientFromEndpoint(
        const udp::endpoint& remote_endpoint)
{
    shared_ptr<ConnectionClient> client = nullptr;
    
    ClientMap::accessor a;

    if (clients_.find(a, remote_endpoint)) {
        client = a->second;
    }

    return client;
}
