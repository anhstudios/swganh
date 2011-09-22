
#include "swganh/object/player/player_factory.h"

#include "swganh/object/player/player.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object::player;

void PlayerFactory::LoadTemplates()
{}

bool PlayerFactory::HasTemplate(const string& template_name)
{
    return false;
}

void PlayerFactory::PersistObject(const shared_ptr<Object>& object)
{}

void PlayerFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{}

shared_ptr<Object> PlayerFactory::CreateObjectFromStorage(uint64_t object_id)
{
    return make_shared<Player>();
}

shared_ptr<Object> PlayerFactory::CreateObjectFromTemplate(const string& template_name)
{
    return make_shared<Player>();
}
