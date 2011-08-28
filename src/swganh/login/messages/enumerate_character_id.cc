#include "swganh/login/messages/enumerate_character_id.h"

using namespace swganh::login;
using namespace messages;
using namespace std;

EnumerateCharacterId swganh::login::messages::BuildEnumerateCharacterId(std::vector<swganh::character::CharacterData> characters) {
    EnumerateCharacterId message;

    message.characters = move(characters);

    return message;
}
