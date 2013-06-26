// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <sstream>

#include <boost/thread/mutex.hpp>

#include "swganh_core/object/object.h"

#include "swganh_core/messages/containers/network_set.h"

#include "guild_tag.h"

namespace swganh {
namespace object {

class GuildFactory;
class GuildMessageBuilder;
class Guild : public swganh::object::Object {
public:
	typedef GuildFactory FactoryType;
	typedef GuildMessageBuilder MessageBuilderType;
    Guild();
    ~Guild();

    void AddGuildTag(uint32_t guild_id, std::string abbreviation);
	void AddGuildTag(uint32_t guild_id, std::string abbreviation, boost::unique_lock<boost::mutex>& lock);

    void RemoveGuildTag(uint32_t guild_id);
	void RemoveGuildTag(uint32_t guild_id, boost::unique_lock<boost::mutex>& lock);

    std::set<GuildTag> GetGuildList();
	std::set<GuildTag> GetGuildList(boost::unique_lock<boost::mutex>& lock);

	void SerializeGuildList(swganh::messages::BaseSwgMessage* message);
	void SerializeGuildList(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock);

    virtual uint32_t GetType() const { return type; }
    const static uint32_t type = 0x444C4947;

private:
	typedef swganh::ValueEvent<std::shared_ptr<Guild>> GuildEvent;
    swganh::containers::NetworkSet<GuildTag>     guild_list_;
};

}}  // namespaces swganh::object
