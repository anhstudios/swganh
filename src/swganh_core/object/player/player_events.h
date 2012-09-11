// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh/event_dispatcher.h"

namespace sql { class ResultSet; }

namespace swganh {
namespace object {

    class Player;

    struct NameEvent : swganh::BaseEvent 
    {
        NameEvent(swganh::EventType type, const std::shared_ptr<Player>& player_, uint64_t name_id_)
            : BaseEvent(type)
            , player(player_)
            , name_id(name_id_){}

        std::shared_ptr<Player> player;
        uint64_t name_id;
    };

    struct WaypointEvent : swganh::BaseEvent
    {
        WaypointEvent(swganh::EventType type, const std::shared_ptr<Player>& player_, const std::shared_ptr<sql::ResultSet> result_set_)
            : BaseEvent(type)
            , player(player_)
            , result_set(result_set_){}
        
        std::shared_ptr<Player> player;
        std::shared_ptr<sql::ResultSet> result_set;
    };



}}
