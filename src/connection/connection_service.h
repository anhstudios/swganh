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

#ifndef CONNECTION_CONNECTION_SERVICE_H_
#define CONNECTION_CONNECTION_SERVICE_H_

#include <map>

#include "anh/network/soe/packet_router.h"
#include "anh/network/soe/server.h"

#include "connection/providers/session_provider_interface.h"
#include "swganh/base/base_service.h"
#include "swganh/character/base_character_service.h"
#include "swganh/login/login_service_interface.h"

#include "swganh/scene/messages/cmd_scene_ready.h"

#include "connection/messages/client_permissions_message.h"
#include "connection/messages/select_character.h"
#include "connection/messages/client_create_character.h"
#include "connection/messages/client_create_character_success.h"
#include "connection/messages/client_create_character_failed.h"
#include "connection/messages/client_random_name_request.h"
#include "connection/messages/client_random_name_response.h"
#include "connection/messages/client_id_msg.h"
#include "connection/messages/heart_beat.h"

namespace anh {
namespace app {
class KernelInterface;
}}  // namespace anh::app

namespace anh {
namespace event_dispatcher {
class EventInterface;
}}  // namespace anh::event_dispatcher

namespace connection {

struct ConnectionClient;

    
class ConnectionService : public swganh::base::BaseService {
public:
    explicit ConnectionService(std::shared_ptr<anh::app::KernelInterface> kernel);
    ~ConnectionService();

    anh::service::ServiceDescription GetServiceDescription();
    
    void DescribeConfigOptions(boost::program_options::options_description& description);

    void onStart();
    void onStop();

    void subscribe();

    std::shared_ptr<swganh::character::BaseCharacterService> character_service();
    void character_service(std::shared_ptr<swganh::character::BaseCharacterService> character_service);
    std::shared_ptr<swganh::login::LoginServiceInterface> login_service();
    void login_service(std::shared_ptr<swganh::login::LoginServiceInterface> login_service);

private:
    void HandleClientIdMsg_(std::shared_ptr<ConnectionClient> client, const connection::messages::ClientIdMsg& message);
    void HandleCmdSceneReady_(std::shared_ptr<ConnectionClient> client, const swganh::scene::messages::CmdSceneReady& message);
    void HandleClientRandomNameRequest_(std::shared_ptr<ConnectionClient> client, const connection::messages::ClientRandomNameRequest& message);
    void HandleClientCreateCharacter_(std::shared_ptr<ConnectionClient> client, const connection::messages::ClientCreateCharacter& message);
    void HandleSelectCharacter_(std::shared_ptr<ConnectionClient> client, const connection::messages::SelectCharacter& message);
    
    void RemoveClient_(std::shared_ptr<anh::network::soe::Session> session);
    void AddClient_(std::shared_ptr<anh::network::soe::Session> session);

    std::unique_ptr<anh::network::soe::Server> soe_server_;
    std::shared_ptr<swganh::character::BaseCharacterService> character_service_;
    std::shared_ptr<swganh::login::LoginServiceInterface> login_service_;
    std::shared_ptr<connection::providers::SessionProviderInterface> session_provider_;
    
    typedef std::map<boost::asio::ip::udp::endpoint, std::shared_ptr<ConnectionClient>> ClientMap;
    ClientMap clients_;

    anh::network::soe::PacketRouter<ClientMap> packet_router_;
    
    std::string listen_address_;
    uint16_t listen_port_;
};

}  // namespace connection
#endif  // CONNECTION_CONNECTION_SERVICE_H_