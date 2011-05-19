#ifndef ANH_PACKETS_FACTION_RESPONSE_MESSAGE_H
#define ANH_PACKETS_FACTION_RESPONSE_MESSAGE_H

#include <packets/BasePacket.h>
#include <algorithm>
#include <list>


// Originates on Server
namespace packets {
struct FactionName {
    uint16_t size;
    std::string name;
};
struct FactionPoints {
    float points;
};

struct FactionResponseMessage  : public BasePacket
{
    FactionResponseMessage (std::shared_ptr<network::Session> session_  = nullptr, std::string& faction_rank_ = std::string(), int32_t rebel_points_ = 0
        , int32_t imperial_points_ = 0, uint32_t faction_names_list_count_ = 0, std::list<FactionName> faction_names_list_ = std::list<FactionName>()
        , uint32_t faction_points_list_count_ = 0, std::list<FactionPoints> faction_points_list_ = std::list<FactionPoints>())
        : BasePacket(session_, CLIENT)
        , faction_rank(faction_rank_)
        , rebel_points(rebel_points_)
        , faction_names_list_count(faction_names_list_count_)
        , faction_names_list(faction_names_list_)
        , faction_points_list_count(faction_points_list_count_)
        , faction_points_list(faction_points_list_)
        {}
    std::string faction_rank;
    int32_t rebel_points;
    int32_t imperial_points;
    uint32_t faction_names_list_count;
    std::list<FactionName> faction_names_list;
    uint32_t faction_points_list_count;
    std::list<FactionPoints> faction_points_list;
};

class FactionResponseMessageEvent : public anh::event_dispatcher::BasicEvent<FactionResponseMessage >{
public:    
    FactionResponseMessageEvent(std::shared_ptr<network::Session> session_  = nullptr, std::string& faction_rank_ = std::string(), int32_t rebel_points_ = 0
        , int32_t imperial_points_ = 0, uint32_t faction_names_list_count_ = 0, std::list<FactionName> faction_names_list_ = std::list<FactionName>()
        , uint32_t faction_points_list_count_ = 0, std::list<FactionPoints> faction_points_list_ = std::list<FactionPoints>())
        : anh::event_dispatcher::BasicEvent<FactionResponseMessage >("FactionResponseMessage"){}
    virtual ~FactionResponseMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<std::string>(faction_rank);
        buffer.write<int32_t>(rebel_points);
        buffer.write<int32_t>(imperial_points);
        buffer.write<uint32_t>(faction_names_list.size());
        // all the factions the player has accumulated points for
        std::for_each(faction_names_list.begin(), faction_names_list.end(), [&buffer] (FactionName faction) {
            buffer.write<uint16_t>(faction.size);
            buffer.write<std::string>(faction.name);
        });
        buffer.write<uint32_t>(faction_points_list.size());
        std::for_each(faction_points_list.begin(), faction_points_list.end(), [&buffer] (FactionPoints faction) {
            buffer.write<float>(faction.points);
        });
    }
};

} // packets

#endif // ANH_PACKETS_FACTION_RESPONSE_MESSAGE_H