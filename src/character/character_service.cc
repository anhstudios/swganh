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
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>

#include <glog/logging.h>

#ifdef WIN32
#include <regex>
#else
#include <boost/regex.hpp>
#endif

#include "anh/database/database_manager.h"

using namespace swganh::character;
using namespace character;
using namespace anh;
using namespace event_dispatcher;
using namespace database;
using namespace std;

CharacterService::CharacterService(shared_ptr<EventDispatcherInterface> event_dispatcher
,shared_ptr<DatabaseManagerInterface> db_manager) 
    : BaseService(event_dispatcher)
    , db_manager_(db_manager) {}

CharacterService::~CharacterService() {}

void CharacterService::onStart() {
}
void CharacterService::onStop() {
}
void CharacterService::Update() {
}
void CharacterService::subscribe() {
}

vector<CharacterData> CharacterService::GetCharactersForAccount(uint64_t account_id) {
    vector<CharacterData> characters;

    auto conn = db_manager_->getConnection("galaxy_db");
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
            character.character_id = result_set->getUInt64("character_id");
            string name = result_set->getString("name");
            character.name = std::wstring(name.begin(), name.end());
            character.race_crc = result_set->getInt("race_crc");
            character.galaxy_id = result_set->getUInt("server_id");
            character.status = result_set->getInt("status");
            characters.push_back(character);
        } while (statement->getMoreResults());
    }

    return characters;
}

bool CharacterService::DeleteCharacter(uint64_t character_id, uint32_t server_id){
    // this actually just archives the character and all their data so it can still be retrieved at a later time
    string sql = "SELECT sf_CharacterDelete(?,?);";
    auto conn = db_manager_->getConnection("galaxy_db");
    auto statement = shared_ptr<sql::PreparedStatement>(conn->prepareStatement(sql));
    statement->setUInt64(1, character_id);
    statement->setUInt(2, server_id);
    int rows_updated = statement->executeUpdate();
    
    return rows_updated > 0;
}
std::wstring CharacterService::GetRandomNameRequest(const std::string& base_model) {

    auto conn = db_manager_->getConnection("galaxy_db");
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
    return L"";
}
bool CharacterService::UpdateCharacterStatus(uint64_t character_id, uint32_t status) {
    auto conn = db_manager_->getConnection("galaxy_db");
    auto statement = std::shared_ptr<sql::PreparedStatement>(
        conn->prepareStatement("update character set status = ? where character_id = ?")
        );
    statement->setUInt(1, status);
    statement->setUInt64(2, character_id);
    int rows_updated = statement->executeUpdate();

    return rows_updated > 0;
}
std::tuple<uint64_t, std::string> CharacterService::CreateCharacter(const CharacterCreateInfo& character_info) {
    auto conn = db_manager_->getConnection("galaxy_db");
    std::shared_ptr<sql::PreparedStatement> statement;
    std::stringstream sql_sf;
    // @TODO add in 2nd parameter as option to select galaxy_id
    sql_sf << "CALL sp_CharacterCreate(?,2,?,?,?,?,?";
    // get bio info
    if (character_info.biography.size() > 0)
    {
        sql_sf << ",\'" << parseBio_(character_info.biography) << "\'";
    }
    // get appearance info
    sql_sf << parseAppearance_(character_info.appearance);
    // get hair
    sql_sf << parseHair_(character_info.hair_model, character_info.hair_customization[1], character_info.hair_customization[2]);
    
    // base model and finish statement
    sql_sf << ",\'" << character_info.base_model << "\')";
    cout << sql_sf.str();
    statement = std::shared_ptr<sql::PreparedStatement>(
        conn->prepareStatement(sql_sf.str())
        );
    statement->setUInt(1, character_info.account_id);
    statement->setString(2, character_info.first_name);
    statement->setString(3, character_info.last_name);
    statement->setString(4, character_info.profession);
    statement->setString(5, character_info.start_city);
    statement->setDouble(6, character_info.scale);

    auto result_set = std::shared_ptr<sql::ResultSet>(statement->executeQuery());
    if (result_set->next())
    {
        uint64_t char_id = result_set->getUInt64(1);
        std::string error_code;
        if (char_id < 8589934593) // error value
        {
             return make_tuple(char_id, error_code);
        }
        // if we get a special character_id back it means there was an error.
        return make_tuple(char_id, setCharacterCreateErrorCode_(char_id));
    }
    return make_tuple(0,"");
}

std::string CharacterService::parseAppearance_(const uint16_t appearance_data[appearance_size])
{
    stringstream ss;
    for(uint32_t i = 0; i < 0x71; i++)
    {
        ss << "," << boost::lexical_cast<string>(appearance_data[i]);
    }
    ss << "," << boost::lexical_cast<string>(appearance_data[171]);
    ss << "," << boost::lexical_cast<string>(appearance_data[172]);
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
    const uint16_t hair_customization_1, const uint16_t hair_customization_2)
{
    stringstream ss;
    if (hair_model.size() > 0)
    {
        ss << ",\'" << hair_model << "\'," << hair_customization_1 << "," << hair_customization_2;
    }
    else
        ss << ",NULL, 0, 0";

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