#include "login/messages/enumerate_character_id.h"

using namespace login;
using namespace messages;
using namespace std;

EnumerateCharacterId login::messages::BuildEnumerateCharacterId(std::vector<swganh::character::CharacterData> characters) {
    EnumerateCharacterId message;

    message.characters = move(characters);

    return message;
}
