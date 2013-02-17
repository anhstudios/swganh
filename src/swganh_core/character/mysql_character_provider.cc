// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "mysql_character_provider.h"

#include <boost/lexical_cast.hpp>

#ifdef WIN32
#include <regex>
#else
#include <boost/regex.hpp>
#endif

#include "character_create.h"

#include "swganh/crc.h"
#include "swganh/app/swganh_kernel.h"
#include "swganh/database/database_manager.h"
#include "swganh/service/service_directory.h"

#include <cppconn/connection.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>

#include "swganh_core/messages/delete_character_reply_message.h"
#include "swganh_core/character/character_data.h"
#include "swganh_core/messages/delete_character_message.h"
#include "swganh_core/messages/client_create_character.h"
#include "swganh_core/messages/client_create_character_success.h"
#include "swganh_core/messages/client_create_character_failed.h"
#include "swganh_core/messages/client_random_name_request.h"
#include "swganh_core/messages/client_random_name_response.h"

#include "swganh_core/object/player/player.h"

#include "swganh/logger.h"

using namespace std;
using namespace swganh;
using namespace swganh::app;
using namespace swganh::database;
using namespace swganh::character;
using namespace swganh::character;
using namespace swganh::messages;

#ifdef WIN32
using std::wregex;
using std::wsmatch;
using std::regex_match;
using std::regex;
using std::regex_search;
#else
using boost::wregex;
using boost::wsmatch;
using boost::regex_match;
using boost::regex;
using boost::regex_search;
#endif

MysqlCharacterProvider::MysqlCharacterProvider(KernelInterface* kernel)
    : CharacterProviderInterface()
    , kernel_(kernel) 
	, py_character_create_(kernel)
{
	auto conn = kernel_->GetDatabaseManager()->getConnection("galaxy");

	// Load each table of restricted names.
	auto statement = std::shared_ptr<sql::PreparedStatement>(
		conn->prepareStatement("CALL sp_LoadProfaneNames();")
		);

	auto result_set = std::unique_ptr<sql::ResultSet>(statement->executeQuery());
	while(result_set->next())
	{
		profane_names_.push_back(result_set->getString("name"));
	}while(statement->getMoreResults());

	statement.reset(conn->prepareStatement("CALL sp_LoadReservedNames();"));
	result_set.reset(statement->executeQuery());
	while(result_set->next())
	{
		reserved_names_.push_back(result_set->getString("name"));
	}while(statement->getMoreResults());

	statement.reset(conn->prepareStatement("CALL sp_LoadFictionallyReservedNames();"));
	result_set.reset(statement->executeQuery());
	while(result_set->next())
	{
		fictionally_reserved_names_.push_back(result_set->getString("name"));
	}while(statement->getMoreResults());

	statement.reset(conn->prepareStatement("CALL sp_LoadRaciallyInnapropriateNames();"));
	result_set.reset(statement->executeQuery());
	while(result_set->next())
	{
        racially_inappropriate_.push_back(result_set->getString("name"));
	}while(statement->getMoreResults());

	statement.reset(conn->prepareStatement("CALL sp_LoadDeveloperNames();"));	
	result_set.reset(statement->executeQuery());
	while(result_set->next())
	{
		developer_names_.push_back(result_set->getString("name"));
	}while(statement->getMoreResults());
}

vector<CharacterData> MysqlCharacterProvider::GetCharactersForAccount(uint64_t account_id) {
    vector<CharacterData> characters;

    try {
        auto conn = kernel_->GetDatabaseManager()->getConnection("galaxy");
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

                character.race_crc = swganh::memcrc(non_shared_template);
                character.galaxy_id = kernel_->GetServiceDirectory()->galaxy().id();
                character.status = 1;
                characters.push_back(character);
            } while (statement->getMoreResults());
        }
    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }

    return characters;
}

bool MysqlCharacterProvider::DeleteCharacter(uint64_t character_id, uint64_t account_id){
    // this actually just archives the character and all their data so it can still be retrieved at a later time
    string sql = "CALL sp_CharacterDelete(?,?);";
    int rows_updated = 0;
    try {
        auto conn = kernel_->GetDatabaseManager()->getConnection("galaxy");
        auto statement = shared_ptr<sql::PreparedStatement>(conn->prepareStatement(sql));
        statement->setUInt64(1, character_id);
        statement->setUInt64(2, account_id);
        auto result_set = std::unique_ptr<sql::ResultSet>(statement->executeQuery());
        if (result_set->next())
        {
           rows_updated = result_set->getInt(1);
        }
		while(statement->getMoreResults());
    }
     catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
    return rows_updated > 0;
}
std::wstring MysqlCharacterProvider::GetRandomNameRequest(const std::string& base_model) {
    try {
        auto conn = kernel_->GetDatabaseManager()->getConnection("galaxy");
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
        }while(statement->getMoreResults());
    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
    return L"";
}
uint16_t MysqlCharacterProvider::GetMaxCharacters(uint64_t player_id) {
    uint16_t max_chars = 2;
    try {
        auto conn = kernel_->GetDatabaseManager()->getConnection("galaxy");
        auto statement = std::shared_ptr<sql::PreparedStatement>(
            conn->prepareStatement("CALL sp_GetMaxCharacters(?);")
            );
        statement->setUInt64(1, player_id);
        auto result_set = std::unique_ptr<sql::ResultSet>(statement->executeQuery());
        if (result_set->next())
        {
            max_chars = result_set->getUInt(1);
        }while(statement->getMoreResults());
    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
    return max_chars;
}

tuple<uint64_t, string> MysqlCharacterProvider::CreateCharacter(const ClientCreateCharacter& character_info, uint32_t account_id) {
    try {
        // A regular expression that searches for a first name and optional sirname.
        // Only letters, and the ' and - characters are allowed. Only 3 instances
        // of the ' and - characters may be in the entire name, which must be between
        // 3 and 16 characters long.
        const wregex p(
            L"^(?!['-])" // confirm the first character is not ' or -
            L"(?=([^'-]*['-]){0,3}[^'-]*$)" // Confirm that no more than 3 instances of ' or - appear
            L"([a-zA-Z][a-z'-]{2,15})"  // Firstname capture group: 3-16 chars must be a-zA-Z or ' or -
            L"(\\s([a-zA-Z][a-z'-]{2,15}))?$"  // Optional sirname group, same restrictions as sirname
        );

        wsmatch m;

        if (! regex_match(character_info.character_name, m, p)) {
            LOG(warning) << "Invalid character name [" << std::string(character_info.character_name.begin(), character_info.character_name.end()) << "]";
            return make_tuple(0,"name_declined_syntax");
        }

        std::wstring first_name = m[2].str();
        std::wstring last_name = m[4].str();

        std::wstring custom_name = first_name;
        if (!last_name.empty())
        {
            custom_name += L" " + last_name;
        }

		uint64_t character_id = py_character_create_.CreateCharacter(custom_name, character_info.starting_profession, character_info.start_location, character_info.height, character_info.biography, 
			character_info.character_customization, character_info.hair_object, character_info.hair_customization, character_info.player_race_iff);
		if (character_id > 0)
		{
			// Setup player account
			std::unique_ptr<sql::PreparedStatement> statement (
				kernel_->GetDatabaseManager()->getConnection("galaxy")->prepareStatement("CALL sp_AddCharacterToAccount(?,?);"));
			statement->setUInt64(1, character_id);
			statement->setUInt(2, account_id);
			statement->execute();

			return make_tuple(character_id, "");
		}
		
	}
	catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }

    return make_tuple(0, "name_declined_internal_error");
}

std::string MysqlCharacterProvider::getCharacterCreateErrorCode_(uint32_t error_code)
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
	LOG(warning) << "Errored occured in CharacterCreate: " << error_string;
    return error_string;
}

uint64_t MysqlCharacterProvider::GetCharacterIdByName(const string& name)
{
    uint64_t character_id = 0;
    try {
        auto conn = kernel_->GetDatabaseManager()->getConnection("galaxy");
        auto statement = std::unique_ptr<sql::PreparedStatement>(
            conn->prepareStatement("sp_GetCharacterIdByName(?,?);")
            );
        statement->setString(1, name + '%');
        statement->setUInt(2, swganh::object::Player::type);
        auto result_set = std::unique_ptr<sql::ResultSet>(statement->executeQuery());
        if (result_set->next())
        {
           character_id = result_set->getUInt64(1);
        }
		while(statement->getMoreResults());
    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
    return character_id;
}

std::tuple<bool, std::string> MysqlCharacterProvider::IsNameAllowed(std::string name)
{
	// Convert name to lower-case.
	boost::to_lower(name);

	// Run name through filters.
	for(std::string restricted_name : profane_names_) {
		if(regex_search(name, regex(restricted_name)))
			return std::tuple<bool, std::string>(false, "name_declined_profane");
	}

	for(std::string restricted_name : reserved_names_) {
		if(regex_search(name, regex(restricted_name)))
			return std::tuple<bool, std::string>(false, "name_declined_reserved");
	}

	for(std::string restricted_name : fictionally_reserved_names_) {
		if(regex_search(name, regex(restricted_name)))
			return std::tuple<bool, std::string>(false, "name_declined_fictionally_reserved");
	}

	for(std::string restricted_name : racially_inappropriate_) {
		if(regex_search(name, regex(restricted_name)))
			return std::tuple<bool, std::string>(false, "name_declined_racially_inappropriate");
	}

	for(std::string restricted_name : developer_names_) {
		if(regex_search(name, regex(restricted_name)))
			return std::tuple<bool, std::string>(false, "name_declined_developer");
	}
	
	return std::tuple<bool, std::string>(true, " ");
}