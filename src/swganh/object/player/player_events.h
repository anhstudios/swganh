
#ifndef SWGANH_OBJECT_PLAYER_PLAYER_EVENTS_H_
#define SWGANH_OBJECT_PLAYER_PLAYER_EVENTS_H_

#include "anh/event_dispatcher.h"

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



}}}

#endif // SWGANH_OBJECT_PLAYER_PLAYER_EVENTS_H_