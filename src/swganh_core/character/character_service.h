// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <tuple>
#include <vector>

#include "swganh/character/character_service_interface.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh_core/messages/delete_character_message.h"
#include "swganh_core/messages/client_create_character.h"
#include "swganh_core/messages/client_random_name_request.h"

namespace swganh { namespace database { class DatabaseManagerInterface; } }

namespace swganh {
namespace connection {
class ConnectionClientInterface;
}}  // namespace swganh::connection

namespace swganh {
namespace login {
class LoginClientInterface;
}}  // namespace swganh::login

namespace swganh {
namespace character {
	class CharacterProviderInterface;
}}
namespace swganh {
namespace character {
    
class CharacterService : public swganh::character::CharacterServiceInterface {
public:
    explicit CharacterService(swganh::app::SwganhKernel* kernel);
    ~CharacterService();
    
    swganh::service::ServiceDescription GetServiceDescription();

    void Startup();

private:
    void HandleClientRandomNameRequest_(
        const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client, 
        swganh::messages::ClientRandomNameRequest* message);

    void HandleClientCreateCharacter_(
        const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client, 
        swganh::messages::ClientCreateCharacter* message);

    void HandleDeleteCharacterMessage_(
        const std::shared_ptr<swganh::login::LoginClientInterface>& login_client, 
        swganh::messages::DeleteCharacterMessage* message);

    std::shared_ptr<swganh::character::CharacterProviderInterface> character_provider_;
    swganh::app::SwganhKernel* kernel_;
};

}}  // namespace swganh::character
