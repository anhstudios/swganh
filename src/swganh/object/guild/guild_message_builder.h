
#ifndef SWGANH_OBJECT_GUILD_GUILD_MESSAGE_BUILDER_H_
#define SWGANH_OBJECT_GUILD_GUILD_MESSAGE_BUIlDER_H_

#include <boost/optional.hpp>
#include <cstdint>
#include "swganh/object/object_message_builder.h"

namespace swganh {
namespace messages {
    struct BaselinesMessage;
}} // swganh::messages

namespace swganh {
namespace object {

    // Forward Declarations
    class Tangible;

namespace guild {

class Guild;
class GuildMessageBuilder : public ObjectMessageBuilder
{
public:
    GuildMessageBuilder(anh::EventDispatcher* dispatcher)
        : ObjectMessageBuilder(dispatcher)
    {
        RegisterEventHandlers();
    }
    virtual void RegisterEventHandlers();
    // deltas
    static void BuildGuildTagsDelta(std::shared_ptr<Guild> guild);

    // baselines
    static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline3(Guild* guild);
    static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline6(Guild* guild);
};

}}} // swganh::object::guild

#endif // SWGANH_OBJECT_GUILD_GUILD_MESSAGE_BUILDER_H_