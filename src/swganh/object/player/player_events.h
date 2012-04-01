
#ifndef SWGANH_OBJECT_PLAYER_PLAYER_EVENTS_H_
#define SWGANH_OBJECT_PLAYER_PLAYER_EVENTS_H_

#include "anh/event_dispatcher.h"

namespace sql { class ResultSet; }

namespace swganh {
namespace object {
namespace player {
    class Player;

    struct NameEvent : anh::BaseEvent 
    {
        NameEvent(anh::EventType type, const std::shared_ptr<Player>& player_, uint64_t name_id_)
            : BaseEvent(type)
            , player(player_)
            , name_id(name_id_){}

        std::shared_ptr<Player> player;
        uint64_t name_id;
    };

    struct WaypointEvent : anh::BaseEvent
    {
        WaypointEvent(anh::EventType type, const std::shared_ptr<Player>& player_, const std::shared_ptr<sql::ResultSet> result_set_)
            : BaseEvent(type)
            , player(player_)
            , result_set(result_set_){}
        
        std::shared_ptr<Player> player;
            std::shared_ptr<sql::ResultSet> result_set;
    };



}}}

#endif // SWGANH_OBJECT_PLAYER_PLAYER_EVENTS_H_