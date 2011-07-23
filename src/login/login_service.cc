/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "login/login_service.h"

#include <glog/logging.h>

#include "anh/app/kernel_interface.h"

#include "anh/database/database_manager.h"

#include "anh/event_dispatcher/basic_event.h"
#include "anh/event_dispatcher/event_dispatcher_interface.h"

#include "anh/network/soe/packet.h"
#include "anh/network/soe/session.h"
#include "anh/network/soe/server.h"

#include "swganh/base/swg_message_handler.h"

#include "login/messages/delete_character_message.h"
#include "login/messages/delete_character_reply_message.h"
#include "login/messages/enumerate_character_id.h"
#include "login/messages/login_client_id.h"
#include "login/messages/login_client_token.h"
#include "login/messages/login_cluster_status.h"
#include "login/messages/login_enum_cluster.h"

#include "login/authentication_manager.h"
#include "login/login_client.h"
#include "login/encoders/sha512_encoder.h"
#include "login/providers/mysql_account_provider.h"

using namespace anh;
using namespace app;
using namespace swganh;
using namespace base;
using namespace character;
using namespace database;
using namespace event_dispatcher;
using namespace login;
using namespace messages;
using namespace std;

LoginService::LoginService(shared_ptr<KernelInterface> kernel) 
    : swganh::base::BaseService(kernel)
    , listen_port_(0) {
    soe_server_.reset(new network::soe::Server(swganh::base::SwgMessageHandler(kernel->GetEventDispatcher())));
    
    auto encoder = make_shared<encoders::Sha512Encoder>(kernel->GetDatabaseManager());

    authentication_manager_ = make_shared<AuthenticationManager>(encoder);
    account_provider_ = make_shared<providers::MysqlAccountProvider>(kernel->GetDatabaseManager());
}

LoginService::~LoginService() {}

service::Service LoginService::GetServiceDescription() {
    service::Service service_description(
        "ANH Login Service",
        "login",
        "0.1",
        listen_address_, 
        0, 
        listen_port_, 
        0);

    return service_description;
}

void LoginService::DescribeConfigOptions(boost::program_options::options_description& description) {
    description.add_options()
        ("service.login.udp_port", boost::program_options::value<uint16_t>(&listen_port_),
            "The port the login service will listen for incoming client connections on")
        ("service.login.address", boost::program_options::value<string>(&listen_address_),
            "The public address the login service will listen for incoming client connections on")
    ;
}

void LoginService::onStart() {
    soe_server_->Start(listen_port_);

    galaxy_status_ = GetGalaxyStatus_();
}

void LoginService::onUpdate() {
    soe_server_->Update();    
}

void LoginService::onStop() {
    soe_server_->Shutdown();
}

void LoginService::subscribe() {
    event_dispatcher()->subscribe("LoginClientId", bind(&LoginService::HandleLoginClientId_, this, placeholders::_1));
    event_dispatcher()->subscribe("DeleteCharacterMessage", bind(&LoginService::HandleDeleteCharacterMessage_, this, placeholders::_1));
}

shared_ptr<BaseCharacterService> LoginService::character_service() {
    return character_service_;
}

void LoginService::character_service(shared_ptr<BaseCharacterService> character_service) {
    character_service_ = character_service;
}

std::vector<GalaxyStatus> LoginService::GetGalaxyStatus_() {
    std::vector<GalaxyStatus> galaxy_status;
    
    auto service_directory = this->service_directory();

    auto galaxy_list = service_directory->getGalaxySnapshot();

    std::for_each(galaxy_list.begin(), galaxy_list.end(), [this, &galaxy_status, &service_directory] (anh::service::Galaxy& galaxy) {
        auto service_list = service_directory->getServiceSnapshot(std::make_shared<anh::service::Galaxy>(galaxy));

        auto it = std::find_if(service_list.begin(), service_list.end(), [] (anh::service::Service& service) {
            return service.type().compare("connection") == 0;
        });

        if (it != service_list.end()) {
            GalaxyStatus status;
            status.address = it->address();
            status.connection_port = it->udp_port();
            status.distance = 0xffff8f80;
            status.galaxy_id = galaxy.id();
            status.max_characters = 2;
            status.max_population = 0x00000cb2;
            status.name = galaxy.name();
            status.ping_port = it->ping_port();
            status.server_population = 10;
            status.status = it->status();

            galaxy_status.push_back(std::move(status));
        }
    });

    return galaxy_status;
}

bool LoginService::HandleLoginClientId_(shared_ptr<EventInterface> incoming_event) {
    auto remote_event = static_pointer_cast<BasicEvent<anh::network::soe::Packet>>(incoming_event);
    
    LoginClientId message;
    message.deserialize(*remote_event->message());

    auto login_client = make_shared<LoginClient>();

    login_client->username(message.username);
    login_client->password(message.password);
    login_client->version(message.client_version);

    auto account = account_provider_->FindByUsername(login_client->username());

    if (! account) {
        DLOG(WARNING) << "Login request for invalid user: " << login_client->username();
        return true;
    }

    if (! authentication_manager_->Authenticate(login_client, account)) {
        DLOG(WARNING) << "Failed login attempt for user: " << login_client->username();
        return true;
    }

    login_client->account(account);

    LoginClientToken token_message;
    token_message.station_id = account->account_id();
        
    const uint8_t data[56] =
    {
        0x20, 0x00, 0x00, 0x00, 
        0x15, 0x00, 0x00, 0x00,
        0x0E, 0xD6, 0x93, 0xDE, 
        0xD2, 0xEF, 0xBF, 0x8E,
        0xA1, 0xAC, 0xD2, 0xEE, 
        0x4C, 0x55, 0xBE, 0x30,
        0x5F, 0xBE, 0x23, 0x0D, 
        0xB4, 0xAB, 0x58, 0xF9,
        0x62, 0x69, 0x79, 0x67, 
        0xE8, 0x10, 0x6E, 0xD3,
        0x86, 0x9B, 0x3A, 0x4A, 
        0x1A, 0x72, 0xA1, 0xFA,
        0x8F, 0x96, 0xFF, 0x9F, 
        0xA5, 0x62, 0x5A, 0x29,
    };

    ByteBuffer buffer(data, sizeof(data));
    token_message.session_key = buffer;
    token_message.station_username = account->username();
    
    remote_event->session()->SendMessage(token_message);

    LoginEnumCluster cluster_message;
    cluster_message.max_account_chars = 2;

    std::for_each(galaxy_status_.begin(), galaxy_status_.end(), [&cluster_message] (GalaxyStatus& status) {            
        Cluster cluster;
        cluster.distance = status.distance;
        cluster.server_id = status.galaxy_id;
        cluster.server_name = status.name;

        cluster_message.servers.push_back(cluster);
    });

    remote_event->session()->SendMessage(cluster_message);

    LoginClusterStatus cluster_status_message;
    
    std::for_each(galaxy_status_.begin(), galaxy_status_.end(), [&cluster_status_message] (GalaxyStatus& status) {  
        ClusterServer cluster_server;
        cluster_server.address = status.address;
        cluster_server.ping_port = status.ping_port;
        cluster_server.conn_port = status.connection_port;
        cluster_server.distance = status.distance;
        cluster_server.status = status.status;
        cluster_server.server_id = status.galaxy_id;
        cluster_server.not_recommended_flag = 0;
        cluster_server.max_chars = status.max_characters;
        cluster_server.max_pop = status.max_population;
        cluster_server.server_pop = status.server_population;
        
        cluster_status_message.servers.push_back(cluster_server);
    });
        
    remote_event->session()->SendMessage(cluster_status_message);

    EnumerateCharacterId character_message;

    auto characters = character_service_->GetCharactersForAccount(account->account_id());
    
    character_message.characters = characters;
    
    remote_event->session()->SendMessage(character_message);

    return true;
}

bool LoginService::HandleDeleteCharacterMessage_(std::shared_ptr<anh::event_dispatcher::EventInterface> incoming_event) {
    auto remote_event = static_pointer_cast<BasicEvent<anh::network::soe::Packet>>(incoming_event);
    
    DeleteCharacterMessage message;
    message.deserialize(*remote_event->message());

    DeleteCharacterReplyMessage reply_message;
    reply_message.failure_flag = 1;

    if (character_service_->DeleteCharacter(message.character_id))
    {
        reply_message.failure_flag = 0;
    }
    remote_event->session()->SendMessage(reply_message);

    return true;
}
