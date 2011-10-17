
#include "swganh/object/guild/guild_factory.h"

#include "swganh/object/guild/guild.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object::guild;

void GuildFactory::LoadTemplates()
{}

bool GuildFactory::HasTemplate(const string& template_name)
{
    return false;
}

void GuildFactory::PersistObject(const shared_ptr<Object>& object)
{}

void GuildFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{}

shared_ptr<Object> GuildFactory::CreateObjectFromStorage(uint64_t object_id)
{
    return make_shared<Guild>();
}

shared_ptr<Object> GuildFactory::CreateObjectFromTemplate(const string& template_name)
{
    return make_shared<Guild>();
}
