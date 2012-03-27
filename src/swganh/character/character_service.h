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

#ifndef SWGANH_CHARACTER_CHARACTER_SERVICE_H_
#define SWGANH_CHARACTER_CHARACTER_SERVICE_H_

#include <cstdint>
#include <tuple>
#include <vector>

#include "swganh/base/base_service.h"

#include "swganh/messages/delete_character_message.h"
#include "swganh/messages/client_create_character.h"
#include "swganh/messages/client_random_name_request.h"

namespace anh { namespace database { class DatabaseManagerInterface; } }

namespace swganh {
namespace connection {
class ConnectionClient;
}}  // namespace swganh::connection

namespace swganh {
namespace login {
class LoginClient;
}}  // namespace swganh::login

namespace swganh {
namespace character {

class CharacterProviderInterface;
    
class CharacterService : public swganh::base::BaseService {
public:
    explicit CharacterService(swganh::app::SwganhKernel* kernel);
    ~CharacterService();
    
    anh::service::ServiceDescription GetServiceDescription();

    void onStart();
    void onStop();

    void subscribe();

private:
    void HandleClientRandomNameRequest_(
        const std::shared_ptr<swganh::connection::ConnectionClient>& client, 
        const swganh::messages::ClientRandomNameRequest& message);
    void HandleClientCreateCharacter_(
        const std::shared_ptr<swganh::connection::ConnectionClient>& client, 
        const swganh::messages::ClientCreateCharacter& message);
    void HandleDeleteCharacterMessage_(
        const std::shared_ptr<swganh::login::LoginClient>& login_client, 
        const swganh::messages::DeleteCharacterMessage& message);

    std::shared_ptr<CharacterProviderInterface> character_provider_;
};

}}  // namespace swganh::character

#endif  // SWGANH_CHARACTER_CHARACTER_SERVICE_H_
