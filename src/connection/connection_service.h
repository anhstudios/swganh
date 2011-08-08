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

#include "anh/network/soe/server.h"

#include "connection/providers/session_provider_interface.h"
#include "swganh/base/base_service.h"
#include "swganh/character/base_character_service.h"

#include "swganh/scene/messages/cmd_scene_ready.h"

#include "swganh/connection/base_connection_service.h"

#include "connection/messages/client_permissions_message.h"
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

namespace swganh {
namespace connection {
struct ConnectionClient;
}}  // namespace swganh::connection;

namespace connection {
    
class ConnectionService : public swganh::connection::BaseConnectionService {
public:
    explicit ConnectionService(std::shared_ptr<anh::app::KernelInterface> kernel);
    ~ConnectionService();

    anh::service::ServiceDescription GetServiceDescription();
    
    void OnDescribeConfigOptions(boost::program_options::options_description& description);
    
    void subscribe();

private:
    void HandleClientIdMsg_(std::shared_ptr<swganh::connection::ConnectionClient> client, const connection::messages::ClientIdMsg& message);
    void HandleCmdSceneReady_(std::shared_ptr<swganh::connection::ConnectionClient> client, const swganh::scene::messages::CmdSceneReady& message);
    
    void RemoveClient_(std::shared_ptr<anh::network::soe::Session> session);
    void AddClient_(uint64_t player_id, std::shared_ptr<swganh::connection::ConnectionClient> client);

    std::shared_ptr<connection::providers::SessionProviderInterface> session_provider_;
};

}  // namespace connection
#endif  // CONNECTION_CONNECTION_SERVICE_H_