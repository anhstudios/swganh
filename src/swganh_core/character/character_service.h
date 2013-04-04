// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <tuple>
#include <vector>

#include "swganh_core/character/character_service_interface.h"

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
   
/**
* The character service is used to handle character creation, deletion and random
* name generation.
*/
class CharacterService : public swganh::character::CharacterServiceInterface {
public:

	/** 
	* Creates a new instance
	*/
	explicit CharacterService(swganh::app::SwganhKernel* kernel);
    virtual ~CharacterService();

    virtual void Initialize();
    virtual void Startup();

private:

	/**
	* Message handler for Random name requests.
	* This will reply with a RandomNameResponse
	*/
    void HandleClientRandomNameRequest_(
        const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client, 
        swganh::messages::ClientRandomNameRequest* message);

	/**
	* Message handler for ClientCreateCharacter.
	* This will respond with a CharacterCreateSuccess or CreateCharacterFailed
	*/
    void HandleClientCreateCharacter_(
        const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client, 
        swganh::messages::ClientCreateCharacter* message);

	/**
	* Message handler for character deletion.
	* This will reply with DeleteCharacterReplyMessage
	*/
    void HandleDeleteCharacterMessage_(
        const std::shared_ptr<swganh::login::LoginClientInterface>& login_client, 
        swganh::messages::DeleteCharacterMessage* message);

    std::shared_ptr<swganh::character::CharacterProviderInterface> character_provider_;
    swganh::app::SwganhKernel* kernel_;
};

}}  // namespace swganh::character
