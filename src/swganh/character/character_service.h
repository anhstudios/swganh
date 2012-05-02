// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_CHARACTER_CHARACTER_SERVICE_H_
#define SWGANH_CHARACTER_CHARACTER_SERVICE_H_

#include <cstdint>
#include <tuple>
#include <vector>

#include "anh/service/service_interface.h"

#include "swganh/app/swganh_kernel.h"
#include "pub14_core/messages/delete_character_message.h"
#include "pub14_core/messages/client_create_character.h"
#include "pub14_core/messages/client_random_name_request.h"

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
    
class CharacterService : public anh::service::ServiceInterface {
public:
    explicit CharacterService(swganh::app::SwganhKernel* kernel);
    ~CharacterService();
    
    anh::service::ServiceDescription GetServiceDescription();

    void Start();

private:
    void HandleClientRandomNameRequest_(
        const std::shared_ptr<swganh::connection::ConnectionClient>& client, 
        swganh::messages::ClientRandomNameRequest message);

    void HandleClientCreateCharacter_(
        const std::shared_ptr<swganh::connection::ConnectionClient>& client, 
        swganh::messages::ClientCreateCharacter message);

    void HandleDeleteCharacterMessage_(
        const std::shared_ptr<swganh::login::LoginClient>& login_client, 
        swganh::messages::DeleteCharacterMessage message);

    std::shared_ptr<CharacterProviderInterface> character_provider_;
    swganh::app::SwganhKernel* kernel_;
};

}}  // namespace swganh::character

#endif  // SWGANH_CHARACTER_CHARACTER_SERVICE_H_
