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

#include "swganh/character/character_service.h"

#include <boost/lexical_cast.hpp>

#include <cppconn/connection.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>

#include <boost/log/trivial.hpp>

#include <iomanip>

#include "anh/crc.h"

#ifdef WIN32
#include <regex>
#else
#include <boost/regex.hpp>
#endif

#include "anh/database/database_manager.h"

#include "anh/network/soe/session.h"
#include "anh/network/soe/server_interface.h"

#include "anh/service/service_directory_interface.h"
#include "anh/service/service_manager.h"

#include "swganh/login/login_service.h"
#include "swganh/login/login_client.h"
#include "swganh/login/account.h"

#include "swganh/connection/connection_service.h"
#include "swganh/connection/connection_client.h"
#include "swganh/messages/heart_beat.h"

#include "swganh/messages/delete_character_reply_message.h"

#include "swganh/app/swganh_kernel.h"

#ifdef WIN32
#else
using boost::wregex;
using boost::wsmatch;
using boost::regex;
using boost::smatch;
using boost::regex_match;
#endif

using namespace anh;
using namespace anh::app;
using namespace anh::database;
using namespace anh::event_dispatcher;
using namespace std;
using namespace swganh::character;
using namespace swganh::connection;
using namespace swganh::login;
using namespace swganh::messages;

CharacterService::CharacterService(KernelInterface* kernel) 
    : BaseService(kernel) {
}

CharacterService::~CharacterService() {}

service::ServiceDescription CharacterService::GetServiceDescription() {
    service::ServiceDescription service_description(
        "ANH Character Service",
        "character",
        "0.1",
        "127.0.0.1", 
        0, 
        0, 
        0);

    return service_description;
}

void CharacterService::onStart() {
}

void CharacterService::onStop() {}

void CharacterService::subscribe() {
    auto connection_service = std::static_pointer_cast<ConnectionService>(kernel()->GetServiceManager()->GetService("ConnectionService"));

    connection_service->RegisterMessageHandler(
        &CharacterService::HandleClientCreateCharacter_, this);
    
    connection_service->RegisterMessageHandler(
        &CharacterService::HandleClientRandomNameRequest_, this);    
      
    auto login_service = std::static_pointer_cast<LoginService>(kernel()->GetServiceManager()->GetService("LoginService"));
  
    login_service->RegisterMessageHandler(
        &CharacterService::HandleDeleteCharacterMessage_, this);
    
}

vector<CharacterData> CharacterService::GetCharactersForAccount(uint64_t account_id) {
    vector<CharacterData> characters;
    
    try {
        auto conn = kernel()->GetDatabaseManager()->getConnection("galaxy");
        auto statement = std::shared_ptr<sql::PreparedStatement>(
            conn->prepareStatement("CALL sp_ReturnAccountCharacters(?);")
            );
        statement->setInt64(1, account_id);
        auto result_set = std::unique_ptr<sql::ResultSet>(statement->executeQuery());

        uint16_t chars_count = result_set->rowsCount();
        
        if (chars_count > 0)
        {
            // this is needed to ensure we don't get commands out of sync errors
            while (result_set->next())
            {
                CharacterData character;
                character.character_id = result_set->getUInt64("id");

                string custom_name = result_set->getString("custom_name");
                character.name = std::wstring(custom_name.begin(), custom_name.end());
                
                std::string non_shared_template = result_set->getString("iff_template");
				if (non_shared_template.size() > 30)
				{
					non_shared_template.erase(23, 7);
				}

                character.race_crc = anh::memcrc(non_shared_template);
                character.galaxy_id = kernel()->GetServiceDirectory()->galaxy().id();
                character.status = 1;
                characters.push_back(character);
            } while (statement->getMoreResults());
        }
    } catch(sql::SQLException &e) {
        BOOST_LOG_TRIVIAL(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        BOOST_LOG_TRIVIAL(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }

    return characters;
}

bool CharacterService::DeleteCharacter(uint64_t character_id, uint64_t account_id){
    // this actually just archives the character and all their data so it can still be retrieved at a later time
    string sql = "CALL sp_CharacterDelete(?,?);";
    int rows_updated = 0;
    try {
        auto conn = kernel()->GetDatabaseManager()->getConnection("galaxy");
        auto statement = shared_ptr<sql::PreparedStatement>(conn->prepareStatement(sql));
        statement->setUInt64(1, character_id);
        statement->setUInt64(2, account_id);
        auto result_set = std::unique_ptr<sql::ResultSet>(statement->executeQuery());
        if (result_set->next())
        {
           rows_updated = result_set->getInt(1);
        }
    }
     catch(sql::SQLException &e) {
        BOOST_LOG_TRIVIAL(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        BOOST_LOG_TRIVIAL(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
    return rows_updated > 0;
}
std::wstring CharacterService::GetRandomNameRequest(const std::string& base_model) {
    try {
        auto conn = kernel()->GetDatabaseManager()->getConnection("galaxy");
        auto statement = std::shared_ptr<sql::PreparedStatement>(
            conn->prepareStatement("CALL sp_CharacterNameCreate(?);")
            );
        statement->setString(1, base_model);
        auto result_set = std::unique_ptr<sql::ResultSet>(statement->executeQuery());
        if (result_set->next())
        {
            std::string str = result_set->getString(1);
            std::wstring wstr(str.begin(), str.end());
            return wstr;
        }
    } catch(sql::SQLException &e) {
        BOOST_LOG_TRIVIAL(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        BOOST_LOG_TRIVIAL(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
    return L"";
}
uint16_t CharacterService::GetMaxCharacters(uint64_t player_id) {
    uint16_t max_chars = 2;
    try {
        auto conn = kernel()->GetDatabaseManager()->getConnection("galaxy");
        auto statement = std::shared_ptr<sql::PreparedStatement>(
            conn->prepareStatement("SELECT max_characters from player_account where id = ?")
            );
        statement->setUInt64(1, player_id);
        auto result_set = std::unique_ptr<sql::ResultSet>(statement->executeQuery());
        if (result_set->next())
        {
            max_chars = result_set->getUInt(1);
        }
    } catch(sql::SQLException &e) {
        BOOST_LOG_TRIVIAL(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        BOOST_LOG_TRIVIAL(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
    return max_chars;
}

std::tuple<uint64_t, std::string> CharacterService::CreateCharacter(const ClientCreateCharacter& character_info, uint32_t account_id) {
    try {
        // A regular expression that searches for a first name and optional sirname.
        // Only letters, and the ' and - characters are allowed. Only 3 instances
        // of the ' and - characters may be in the entire name, which must be between
        // 3 and 16 characters long.
        const wregex p(
            L"(?!['-])" // confirm the first character is not ' or -
            L"(?!(.*['-]){4,})" // Confirm that no more than 3 instances of ' or - appear
            L"([a-zA-Z][a-z'-]{2,15})"  // Firstname capture group: 3-16 chars must be a-zA-Z or ' or -
            L"(\\s([a-zA-Z][a-z'-]{2,15}))?"  // Optional sirname group, same restrictions as sirname
        );

        wsmatch m;

        if (! regex_match(character_info.character_name, m, p)) {
            BOOST_LOG_TRIVIAL(warning) << "Invalid character name [" << std::string(character_info.character_name.begin(), character_info.character_name.end()) << "]";
            return make_tuple(0,"name_declined_syntax");
        }

        std::wstring first_name = m[2].str();
        std::wstring last_name = m[4].str();

        std::wstring custom_name = first_name;
        if (!last_name.empty())
        {
            custom_name += L" " + last_name;
        }

        auto conn = kernel()->GetDatabaseManager()->getConnection("galaxy");

        std::unique_ptr<sql::PreparedStatement> statement(conn->prepareStatement(
            "CALL sp_CharacterCreate(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, @output)"));

        BOOST_LOG_TRIVIAL(warning) << "Creating character with location " << account_id;

        statement->setUInt(1, account_id);
        statement->setUInt(2, kernel()->GetServiceDirectory()->galaxy().id());
        statement->setString(3, string(first_name.begin(), first_name.end()));
        statement->setString(4, string(last_name.begin(), last_name.end()));
        statement->setString(5, string(custom_name.begin(), custom_name.end()));
        statement->setString(6, character_info.starting_profession);
        statement->setString(7, character_info.start_location);
        statement->setDouble(8, character_info.height);
        statement->setString(9, character_info.biography);
        statement->setString(10, character_info.character_customization);
        statement->setString(11, character_info.hair_object);
        statement->setString(12, character_info.hair_customization);
        statement->setString(13, character_info.player_race_iff);

        statement->execute();        

        statement.reset(conn->prepareStatement("SELECT @output as _object_id"));

        auto result_set = std::unique_ptr<sql::ResultSet>(statement->executeQuery());
        if (result_set->next())
        {
            uint64_t char_id = result_set->getUInt64(1);
            if (char_id < 1002) 
            {
                // if we get a special character_id back it means there was an error.
                // @TODO Change this to return a separate output value for the error code
                return make_tuple(0, setCharacterCreateErrorCode_(static_cast<uint32_t>(char_id)));
            }
            return make_tuple(char_id, "");
        }
    } 
    catch(sql::SQLException &e) 
    {
        BOOST_LOG_TRIVIAL(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        BOOST_LOG_TRIVIAL(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }

    return make_tuple(0, "name_declined_internal_error");
}

std::string CharacterService::setCharacterCreateErrorCode_(uint32_t error_code)
{
    std::string error_string;
    switch(error_code)
    {
    case 0:
        error_string = "name_declined_developer";
        break;

    case 1:
        error_string = "name_declined_empty";
        break;

    case 2:
        error_string = "name_declined_fictionally_reserved";
        break;

    case 3:
        error_string = "name_declined_in_use";
        break;

    case 4:
        error_string = "name_declined_internal_error";
        break;

    case 5:
        error_string = "name_declined_no_name_generator";
        break;

    case 6:
        error_string = "name_declined_no_template";
        break;

    case 7:
        error_string = "name_declined_not_authorized_for_species";
        break;

    case 8:
        error_string = "name_declined_not_creature_template";
        break;

    case 9:
        error_string = "name_declined_not_authorized_for_species";
        break;

    case 10:
        error_string = "name_declined_number";
        break;

    case 11:
        error_string = "name_declined_profane";
        break;

    case 12:
        error_string = "name_declined_racially_inappropriate";
        break;

    case 13:
        error_string = "name_declined_reserved";
        break;

    case 14:
        error_string = "name_declined_retry";
        break;

    case 15:
        error_string = "name_declined_syntax";
        break;

    case 16:
        error_string = "name_declined_too_fast";
        break;

    case 17:
        error_string = "name_declined_cant_create_avatar";
        break;

    default:
        error_string = "name_declined_internal_error";
        break;
    }
    return error_string;
}

void CharacterService::HandleClientCreateCharacter_(const std::shared_ptr<ConnectionClient>& client, const ClientCreateCharacter& message) {
    BOOST_LOG_TRIVIAL(warning) << "Handling ClientCreateCharacter";

    uint64_t character_id;
    string error_code;
    tie(character_id, error_code) = CreateCharacter(message, client->GetAccountId());

    // heartbeat to let the client know we're still here    
    HeartBeat heartbeat;
    client->SendMessage(heartbeat);

    if (error_code.length() > 0 && character_id == 0) {
        ClientCreateCharacterFailed failed;
        failed.stf_file = "ui";
        failed.error_string = error_code;
        client->SendMessage(failed);
    } else {
        ClientCreateCharacterSuccess success;
        success.character_id = character_id;
        client->SendMessage(success);
    }
}

void CharacterService::HandleClientRandomNameRequest_(const std::shared_ptr<ConnectionClient>& client, const ClientRandomNameRequest& message) {
    ClientRandomNameResponse response;
    response.player_race_iff = message.player_race_iff;
    
    response.random_name = GetRandomNameRequest(message.player_race_iff);
    if (response.random_name.length() > 0) {
        response.stf_file = "ui";
        response.approval_string = "name_approved";
    }

    client->SendMessage(response);
}

void CharacterService::HandleDeleteCharacterMessage_(const std::shared_ptr<LoginClient>& login_client, const DeleteCharacterMessage& message) {
    DeleteCharacterReplyMessage reply_message;
    reply_message.failure_flag = 1;

    if (DeleteCharacter(message.character_id, login_client->GetAccount()->account_id())) {
        reply_message.failure_flag = 0;
    }

    login_client->SendMessage(reply_message);
}
