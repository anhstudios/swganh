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

#ifndef LOGIN_LOGIN_SERVICE_H_
#define LOGIN_LOGIN_SERVICE_H_

#include <map>

#include <boost/asio.hpp>

#include <glog/logging.h>

#include "anh/network/soe/packet_router.h"
#include "anh/network/soe/server.h"

#include "swganh/base/base_service.h"
#include "swganh/character/base_character_service.h"

#include "login/galaxy_status.h"
#include "login/messages/login_client_id.h"
#include "login/messages/delete_character_message.h"

namespace anh {
namespace app {
class KernelInterface;
}}  // namespace anh::app

namespace anh {
namespace network {
namespace soe {
class Server;
}}}  // namespace anh::network::soe

namespace anh {
namespace database {
class DatabaseManagerInterface; 
}}  // namespace anh::database

namespace anh {
namespace event_dispatcher {
class EventInterface;
}}  // namespace anh::event_dispatcher

namespace login {
    
class AuthenticationManager;

namespace providers {
class AccountProviderInterface;
}

struct LoginClient;

class LoginService : public swganh::base::BaseService {
public:
    explicit LoginService(std::shared_ptr<anh::app::KernelInterface> kernel);
    ~LoginService();
    
    anh::service::ServiceDescription GetServiceDescription();

    void DescribeConfigOptions(boost::program_options::options_description& description);

private:
    LoginService();

    void onStart();
    void onStop();
    void onUpdate();

    void subscribe();
    
    bool HandleLoginClientId_(std::shared_ptr<anh::event_dispatcher::EventInterface> incoming_event);
    void HandleDeleteCharacterMessage_(std::shared_ptr<LoginClient> login_client, const messages::DeleteCharacterMessage& message);

    std::vector<GalaxyStatus> GetGalaxyStatus_();
    void UpdateGalaxyStatus_();
    
    std::unique_ptr<anh::network::soe::Server> soe_server_;
    std::shared_ptr<swganh::character::BaseCharacterService> character_service_;
    std::shared_ptr<AuthenticationManager> authentication_manager_;
    std::shared_ptr<providers::AccountProviderInterface> account_provider_;
    
    template<typename MessageType>
    bool HandleRemoteMessage(
        std::shared_ptr<anh::event_dispatcher::EventInterface> incoming_event,
        //void (LoginService::*handler) (std::shared_ptr<LoginClient>, const MessageType&))
        std::function<void (std::shared_ptr<LoginClient>, const MessageType&)> handler)
    {
        auto remote_event = std::static_pointer_cast<anh::event_dispatcher::BasicEvent<anh::network::soe::Packet>>(incoming_event);
    
        auto it = clients_.find(remote_event->session()->remote_endpoint());
        
        if (it == clients_.end()) {
            throw std::runtime_error("Received remote message for an invalid client");
        } 

        auto login_client = it->second;

        try {
            MessageType message;
            message.deserialize(*remote_event->message());        
            
            handler(login_client, message);
        } catch(const std::exception& e) {
            DLOG(ERROR) << "Error handling remote message\nclient: " << login_client->username << "\nerror: \n" << e.what();
            return false;
        }

        return true;
    }

    std::vector<GalaxyStatus> galaxy_status_;
    
    int galaxy_status_check_duration_secs_;
    boost::asio::deadline_timer galaxy_status_timer_;

    typedef std::map<boost::asio::ip::udp::endpoint, std::shared_ptr<LoginClient>> ClientMap;
    ClientMap clients_;
    
    anh::network::soe::PacketRouter<ClientMap> packet_router_;

    std::string listen_address_;
    uint16_t listen_port_;
};

}  // namespace login
#endif  // LOGIN_LOGIN_SERVICE_H_