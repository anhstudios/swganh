
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

BaseConnectionService::BaseConnectionService(
        string listen_address, 
        uint16_t listen_port, 
        std::shared_ptr<anh::app::KernelInterface> kernel)
    : BaseService(kernel)
#pragma warning(push)
#pragma warning(disable: 4355)
    , SwgMessageRouter([=] (const boost::asio::ip::udp::endpoint& endpoint) {
        return GetClientFromEndpoint(endpoint);  
      })
#pragma warning(pop)
    , listen_address_(listen_address)
    , listen_port_(listen_port)
    , soe_server_(nullptr)
{
    soe_server_.reset(new soe::Server(
        kernel->GetIoService(),
        bind(&BaseConnectionService::RouteMessage, this, placeholders::_1)));
    soe_server_->event_dispatcher(kernel->GetEventDispatcher());
}

void BaseConnectionService::onStart() {
    soe_server_->Start(listen_port_);
    
    character_service_ = std::static_pointer_cast<BaseCharacterService>(kernel()->GetServiceManager()->GetService("CharacterService"));    
    login_service_ = std::static_pointer_cast<swganh::login::BaseLoginService>(kernel()->GetServiceManager()->GetService("LoginService"));
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

shared_ptr<BaseLoginService> BaseConnectionService::login_service() {
    return login_service_;
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
