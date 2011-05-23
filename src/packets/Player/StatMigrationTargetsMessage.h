#ifndef ANH_PACKETS_STAT_MIGRATION_TARGETS_MESSAGE_H
#define ANH_PACKETS_STAT_MIGRATION_TARGETS_MESSAGE_H

#include <packets/BasePacket.h>
#include <algorithm>
#include <list>

// Originates on Server
namespace packets {
struct StatMigrationTargetsMessage  : public BasePacket
{
    StatMigrationTargetsMessage (std::shared_ptr<anh::network::soe::Session> session_  = nullptr, int32_t health_ = 0
    ,int32_t strength_ = 0, int32_t constitution_ = 0, int32_t action_ = 0,int32_t quickness_ = 0, int32_t stamina_ = 0
    ,int32_t mind_ = 0, int32_t focus_ = 0, int32_t willpower_ = 0,int32_t points_left_ = 0)
        : BasePacket(session_, CLIENT)
        , health(health_)
        , strength(strength_)
        , constitution(constitution_)
        , action(action_)
        , quickness(quickness_)
        , stamina(stamina_)
        , mind(mind_)
        , focus(focus_)
        , willpower(willpower_)
        , points_left(points_left_)
        {}
    int32_t health;
    int32_t strength;
    int32_t constitution;
    int32_t action;
    int32_t quickness;
    int32_t stamina;
    int32_t mind;
    int32_t focus;
    int32_t willpower;
    int32_t points_left;
};

class StatMigrationTargetsMessageEvent : public anh::event_dispatcher::BasicEvent<StatMigrationTargetsMessage>{
public:    
    StatMigrationTargetsMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, int32_t health_ = 0
    ,int32_t strength_ = 0, int32_t constitution_ = 0, int32_t action_ = 0,int32_t quickness_ = 0, int32_t stamina_ = 0
    ,int32_t mind_ = 0, int32_t focus_ = 0, int32_t willpower_ = 0,int32_t points_left_ = 0) 
        : anh::event_dispatcher::BasicEvent<StatMigrationTargetsMessage>("StatMigrationTargetsMessage"){}
    virtual ~StatMigrationTargetsMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<int32_t>(health);
        buffer.write<int32_t>(strength);
        buffer.write<int32_t>(constitution);
        buffer.write<int32_t>(action);
        buffer.write<int32_t>(quickness);
        buffer.write<int32_t>(stamina);
        buffer.write<int32_t>(mind);
        buffer.write<int32_t>(focus);
        buffer.write<int32_t>(willpower);
        buffer.write<int32_t>(points_left);
    }
};

} // packets

#endif // ANH_PACKETS_STAT_MIGRATION_TARGETS_MESSAGE_H