// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_CHARACTER_CHARACTER_SERVICE_INTERFACE_H_
#define SWGANH_CHARACTER_CHARACTER_SERVICE_INTERFACE_H_

#include <cstdint>
#include <tuple>
#include <vector>

#include "anh/service/service_interface.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh/messages/delete_character_message.h"
#include "swganh/messages/client_create_character.h"
#include "swganh/messages/client_random_name_request.h"

namespace anh { namespace database { class DatabaseManagerInterface; } }

namespace swganh {
namespace connection {
class ConnectionClientInterface;
}}  // namespace swganh::connection

namespace swganh {
namespace login {
class LoginClient;
}}  // namespace swganh::login

namespace swganh {
namespace character {

class CharacterProviderInterface;
    
class CharacterServiceInterface : public anh::service::ServiceInterface {
private:
    virtual void HandleClientRandomNameRequest_(
        const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client, 
        swganh::messages::ClientRandomNameRequest message) = 0;

    virtual void HandleClientCreateCharacter_(
        const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client, 
        swganh::messages::ClientCreateCharacter message) = 0;

    virtual void HandleDeleteCharacterMessage_(
        const std::shared_ptr<swganh::login::LoginClient>& login_client, 
        swganh::messages::DeleteCharacterMessage message) = 0;
};

}}  // namespace swganh::character

#endif  // SWGANH_CHARACTER_CHARACTER_SERVICE_INTERFACE_H_
