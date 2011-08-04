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

#include "connection/connection_service.h"

#include <iostream>
#include <tuple>
#include <glog/logging.h>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "anh/crc.h"
#include "anh/app/kernel_interface.h"
#include "anh/event_dispatcher/basic_event.h"
#include "anh/event_dispatcher/event_dispatcher_interface.h"
#include "anh/plugin/plugin_manager.h"
#include "anh/service/service_manager.h"

#include "anh/network/soe/packet.h"
#include "anh/network/soe/session.h"

#include "character/character_service.h"

#include "swganh/base/swg_message_handler.h"
#include "swganh/character/character_data.h"

#include "swganh/scene/messages/cmd_scene_ready.h"
#include "swganh/scene/messages/cmd_start_scene.h"
#include "swganh/scene/messages/scene_create_object_by_crc.h"
#include "swganh/scene/messages/scene_end_baselines.h"

#include "connection/providers/mysql_session_provider.h"

#include "connection/messages/client_permissions_message.h"
#include "connection/messages/client_id_msg.h"
#include "connection/messages/heart_beat.h"

#include "swganh/connection/connection_client.h"

using namespace anh;
using namespace app;
using namespace event_dispatcher;
using namespace connection;
using namespace swganh::login;
using namespace messages;
using namespace swganh::base;
using namespace swganh::character;
using namespace swganh::connection;
using namespace swganh::scene::messages;
using namespace std;

ConnectionService::ConnectionService(shared_ptr<KernelInterface> kernel) 
    : swganh::connection::BaseConnectionService(kernel)
{        
    session_provider_ = make_shared<connection::providers::MysqlSessionProvider>(kernel->GetDatabaseManager());
}

ConnectionService::~ConnectionService() {}

service::ServiceDescription ConnectionService::GetServiceDescription() {
    service::ServiceDescription service_description(
        "ANH Connection Service",
        "connection",
        "0.1",
        listen_address(), 
        0,
        listen_port(), 
        0);

    return service_description;
}

void ConnectionService::OnDescribeConfigOptions(
    boost::program_options::options_description& description) 
{}

void ConnectionService::subscribe() {
    auto event_dispatcher = kernel()->GetEventDispatcher();
     
    RegisterMessageHandler<ClientIdMsg>(
        bind(&ConnectionService::HandleClientIdMsg_, this, placeholders::_1, placeholders::_2), false);

    RegisterMessageHandler<CmdSceneReady>(
        bind(&ConnectionService::HandleCmdSceneReady_, this, placeholders::_1, placeholders::_2));

    event_dispatcher->subscribe("NetworkSessionRemoved", [this] (shared_ptr<EventInterface> incoming_event) -> bool {
        auto session_removed = std::static_pointer_cast<anh::event_dispatcher::BasicEvent<anh::network::soe::SessionData>>(incoming_event);
        
        // Message was triggered from our server so process it.
        if (session_removed->session->server() == server().get()) {
            RemoveClient_(session_removed->session);
        }

        return true;
    });
}

void ConnectionService::AddClient_(
    uint64_t player_id, 
    std::shared_ptr<ConnectionClient> client) 
{
    ClientMap& client_map = clients();

    auto find_it = std::find_if(
        client_map.begin(), 
        client_map.end(), 
        [client, player_id] (ClientMap::value_type& conn_client) 
    {
        return conn_client.first.address() == client->session->remote_endpoint().address() 
            && conn_client.second->player_id == player_id;
    });

    if (find_it != client_map.end()) {
        // client already exists, lets boot the existing one and let the requesting one continue
        // @TODO: Make this configurable
        (*find_it).second->session->Close();

        client_map.erase(find_it->first);
    }
    
    DLOG(WARNING) << "Adding connection client";

    ClientMap::accessor a;
    client_map.insert(a, client->session->remote_endpoint());
    a->second = client;

    DLOG(WARNING) << "Connection service currently has ("<< client_map.size() << ") clients";
}


void ConnectionService::RemoveClient_(std::shared_ptr<anh::network::soe::Session> session) {
    active().Async([=] () {
        auto client = GetClientFromEndpoint(session->remote_endpoint());
        
        auto client_map = clients();

        if (client) {
            DLOG(WARNING) << "Removing disconnected client";
            client_map.erase(session->remote_endpoint());
        }
                
        DLOG(WARNING) << "Connection service currently has ("<< client_map.size() << ") clients";
    });
}

void ConnectionService::HandleCmdSceneReady_(std::shared_ptr<ConnectionClient> client, const CmdSceneReady& message) {
    DLOG(WARNING) << "Handling CmdSceneReady";
    
    client->session->SendMessage(CmdSceneReady());
}

void ConnectionService::HandleClientIdMsg_(std::shared_ptr<ConnectionClient> client, const ClientIdMsg& message) {
    DLOG(WARNING) << "Handling ClientIdMsg";

    // get session key from login service
    uint32_t account_id = login_service()->GetAccountBySessionKey(message.session_hash);

    // authorized
    if (! account_id) {
        DLOG(WARNING) << "Account_id not found from session key, unauthorized access.";
        return;
    }
    
    // gets player from account
    uint64_t player_id = session_provider_->GetPlayerId(account_id);
                
    // authorized
    if (! player_id) {
        DLOG(WARNING) << "No player found for the requested account, unauthorized access.";
        return;
    }
    
    // creates a new session and stores it for later use
    if (!session_provider_->CreateGameSession(player_id, client->session->connection_id())) {    
        DLOG(WARNING) << "Player Not Inserted into Session Map because No Game Session Created!";
    }
    
    client->account_id = account_id;
    client->player_id = player_id;

    AddClient_(player_id, client);

    ClientPermissionsMessage client_permissions;
    client_permissions.galaxy_available = service_directory()->galaxy().status();
    client_permissions.available_character_slots = character_service()->GetMaxCharacters(account_id);
    // @TODO: Replace with configurable value
    client_permissions.unlimited_characters = 0;

    client->session->SendMessage(client_permissions);
}
