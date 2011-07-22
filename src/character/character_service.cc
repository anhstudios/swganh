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

#include "character/character_service.h"

#include <boost/lexical_cast.hpp>

#include <cppconn/connection.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>

#include <glog/logging.h>

#include <iomanip>

#include "anh/crc.h"

#ifdef WIN32
#include <regex>
#else
#include <boost/regex.hpp>
#endif

#include "anh/app/kernel_interface.h"
#include "anh/database/database_manager.h"

#ifdef WIN32
#else
using boost::wregex;
using boost::wsmatch;
using boost::regex_match;
#endif

using namespace swganh::character;
using namespace character;
using namespace connection::messages;
using namespace anh;
using namespace app;
using namespace event_dispatcher;
using namespace database;
using namespace std;

CharacterService::CharacterService(shared_ptr<KernelInterface> kernel) 
    : BaseCharacterService(kernel) {
}

CharacterService::~CharacterService() {}

void CharacterService::onStart() {
}
void CharacterService::onStop() {
}
void CharacterService::Update() {
}
void CharacterService::subscribe() {
}

void CharacterService::DescribeConfigOptions(boost::program_options::options_description& description) {
    description.add_options()
        ("db.swg_manager.schema", boost::program_options::value<std::string>(&swg_schema_),
            "The schema that is used to manage the swg simulation")
        ("db.galaxy_manager.schema", boost::program_options::value<std::string>(&galaxy_schema_),
            "The schema that is used to manage the galaxy")
    ;
}

vector<CharacterData> CharacterService::GetCharactersForAccount(uint64_t account_id) {
    vector<CharacterData> characters;
    
    try {
        auto conn = kernel()->GetDatabaseManager()->getConnection("galaxy");
        auto statement = std::shared_ptr<sql::PreparedStatement>(
            conn->prepareStatement("CALL sp_ReturnAccountCharacters(?);")
            );
        statement->setInt64(1, account_id);
        auto result_set = std::shared_ptr<sql::ResultSet>(statement->executeQuery());

        uint16_t chars_count = result_set->rowsCount();
        
        if (chars_count > 0)
        {
            // this is needed to ensure we don't get commands out of sync errors
            while (result_set->next())
            {
                CharacterData character;
                character.character_id = result_set->getUInt64("id");
                string name = result_set->getString("firstName") + " " + result_set->getString("lastName");
                character.name = std::wstring(name.begin(), name.end());
                character.race_crc = anh::memcrc(result_set->getString("baseModel"));
                character.galaxy_id = 1 /* we'll get this from the service */;
                character.status = result_set->getInt("jediState");
                characters.push_back(character);
            } while (statement->getMoreResults());
        }
    } catch(sql::SQLException &e) {
        DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }

    return characters;
}
CharacterLoginData CharacterService::GetLoginCharacter(uint64_t character_id) {
    CharacterLoginData character;
    
    try {
        string sql = "CALL sp_GetLoginCharacter(?);";
        auto conn = kernel()->GetDatabaseManager()->getConnection("galaxy");
        auto statement = shared_ptr<sql::PreparedStatement>(conn->prepareStatement(sql));
        statement->setUInt64(1, character_id);

        auto result_set = statement->executeQuery();
        if (result_set->next())
        {
            character.character_id = result_set->getUInt64("id");
            character.position.x = result_set->getDouble("x");
            character.position.y = result_set->getDouble("y");
            character.position.z = result_set->getDouble("z");
            character.orientation.x = result_set->getDouble("oX");
            character.orientation.y = result_set->getDouble("oY");
            character.orientation.z = result_set->getDouble("oZ");
            character.orientation.w = result_set->getDouble("oW");
            character.race_template = result_set->getString("baseModel");
            if (result_set->getInt("gender") == 0)
                character.gender = "female";
            else
                character.gender = "male";
            character.race = result_set->getString("species");
            character.terrain_map = result_set->getString("terrainMap");
        }
    } catch(sql::SQLException &e) {
        DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
    return character;
}
bool CharacterService::DeleteCharacter(uint64_t character_id){
    // this actually just archives the character and all their data so it can still be retrieved at a later time
    string sql = "CALL sp_CharacterDelete(?);";
    int rows_updated = 0;
    try {
        auto conn = kernel()->GetDatabaseManager()->getConnection("galaxy");
        auto statement = shared_ptr<sql::PreparedStatement>(conn->prepareStatement(sql));
        statement->setUInt64(1, character_id);
        auto result_set = std::shared_ptr<sql::ResultSet>(statement->executeQuery());
        if (result_set->next())
        {
           rows_updated = result_set->getInt(1);
        }
    }
     catch(sql::SQLException &e) {
        DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
    return rows_updated > 0;
}
std::wstring CharacterService::GetRandomNameRequest(const std::string& base_model) {
    try {
        auto conn = kernel()->GetDatabaseManager()->getConnection("galaxy");
        auto statement = std::shared_ptr<sql::PreparedStatement>(
            conn->prepareStatement("SELECT sf_CharacterNameCreate(?);")
            );
        statement->setString(1, base_model);
        auto result_set = std::shared_ptr<sql::ResultSet>(statement->executeQuery());
        if (result_set->next())
        {
            std::string str = result_set->getString(1);
            std::wstring wstr(str.begin(), str.end());
            return wstr;
        }
    } catch(sql::SQLException &e) {
        DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
    return L"";
}
bool CharacterService::UpdateCharacterStatus(uint64_t character_id, uint32_t status) {
    int rows_updated = 0;
    try {
        auto conn = kernel()->GetDatabaseManager()->getConnection("galaxy");
        auto statement = std::shared_ptr<sql::PreparedStatement>(
            conn->prepareStatement("update character set status = ? where character_id = ?")
            );
        statement->setUInt(1, status);
        statement->setUInt64(2, character_id);
        rows_updated = statement->executeUpdate();

     } catch(sql::SQLException &e) {
        DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }


    return rows_updated > 0;
}
std::tuple<uint64_t, std::string> CharacterService::CreateCharacter(const ClientCreateCharacter& character_info) {
    try {
        // A regular expression that searches for a first name and optional sirname.
        // Only letters, and the ' and - characters are allowed. Only 3 instances
        // of the ' and - characters may be in the entire name, which must be between
        // 3 and 16 characters long.
        const wregex p(L"(?!['-])(?!.*['-].*['-].*['-].*['-])([a-zA-Z][a-z'-]{3,16}?)(?: ([a-zA-Z][a-z'-]{3,16}?))?");
        wsmatch m;

        if (! regex_match(character_info.character_name, m, p)) {
            LOG(WARNING) << "Invalid character name [" << std::string(character_info.character_name.begin(), character_info.character_name.end()) << "]";
            return make_tuple(0,"name_declined_syntax");
        }
        std::wstring first_name = m[1].str();
        std::wstring last_name = L"";
        if (m[2].matched) {
            last_name = m[2].str();
        }

        auto conn = kernel()->GetDatabaseManager()->getConnection("galaxy");
        std::shared_ptr<sql::PreparedStatement> statement;
        std::stringstream sql_sf;
        // @TODO add in 2nd parameter as option to select galaxy_id
        sql_sf << "CALL sp_CharacterCreate(?,2,?,?,?,?,?";
        // get bio info
        if (character_info.biography.size() > 0)
        {
            sql_sf << ",\'" << parseBio_(character_info.biography) << "\'";
        }
        else
        {
            sql_sf << ", \'\'";
        }
        // get appearance info
        sql_sf << parseAppearance_(character_info.character_customization);
        // get hair
        sql_sf << parseHair_(character_info.hair_object, character_info.hair_customization);
    
        // base model and finish statement
        sql_sf << ",\'" << character_info.player_race_iff << "\');";
        statement = std::shared_ptr<sql::PreparedStatement>(
            conn->prepareStatement(sql_sf.str())
            );
        statement->setUInt(1, 1 /*character_info.account_id*/);
        statement->setString(2, string(first_name.begin(), first_name.end()));
        statement->setString(3, string(last_name.begin(), last_name.end()));
        statement->setString(4, character_info.starting_profession);
        statement->setString(5, character_info.start_location);
        statement->setDouble(6, character_info.height);

        auto result_set = std::shared_ptr<sql::ResultSet>(statement->executeQuery());
        if (result_set->next())
        {
            uint64_t char_id = result_set->getUInt64(1);
            if (char_id > 0) // error value
            {
                // if we get a special character_id back it means there was an error.
                return make_tuple(char_id, setCharacterCreateErrorCode_(char_id));
            }
            return make_tuple(char_id, "");
        }
    } catch(sql::SQLException &e) {
        DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
    return make_tuple(0,"");
}

std::string CharacterService::parseAppearance_(std::string appearance_data)
{
    stringstream ss;
    ss << ", \'";
    ss << setfill('0') << setw(2);
    std::for_each(appearance_data.begin(), appearance_data.end(), [&ss] (uint16_t data) {
        cout << std::hex << data;
        ss << std::hex << data;
    });
    ss << "\'";
    return ss.str();
}
std::string CharacterService::parseBio_(const std::string& bio)
{
    //regex_replace
    std::string parsed_bio = regex_replace(bio, regex("'"), std::string("''"));    
    // escape the escape characters - necessary for how regex and C++ works...
    parsed_bio = regex_replace(parsed_bio, regex("\\\\"), std::string("\\\\"));
    return parsed_bio;
}
std::string CharacterService::parseHair_(const std::string& hair_model,
    const std::string& hair_customization)
{
    stringstream ss;
    if (hair_model.size() > 0)
    {
        ss << ",\'" << hair_model << "\', \'" ;
        ss << setfill('0') << setw(2);
        std::for_each(hair_customization.begin(), hair_customization.end(), [&ss] (uint16_t data) {
            ss << std::hex << data;
        });
        ss << "\'";
    }
    else
        ss << ",NULL, 0";

    return ss.str();
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