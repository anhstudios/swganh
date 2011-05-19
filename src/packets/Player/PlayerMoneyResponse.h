#ifndef ANH_PACKETS_PLAYER_MONEY_RESPONSE_H
#define ANH_PACKETS_PLAYER_MONEY_RESPONSE_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct PlayerMoneyResponse  : public BasePacket
{
    PlayerMoneyResponse (std::shared_ptr<network::Session> session_  = nullptr, int32_t credits_bank_ = 0
        , int32_t credits_inventory_ = 0)
        : BasePacket(session_, CLIENT)
        , credits_bank(credits_bank_)
        , credits_inventory(credits_inventory_)
        {}
    int32_t credits_bank;
    int32_t credits_inventory;
};

class PlayerMoneyResponseEvent : public anh::event_dispatcher::BasicEvent<PlayerMoneyResponse >{
public:    
    PlayerMoneyResponseEvent(std::shared_ptr<network::Session> session_  = nullptr, int32_t credits_bank = 0
        , int32_t credits_inventory = 0) 
        : anh::event_dispatcher::BasicEvent<PlayerMoneyResponse >("PlayerMoneyResponse"){}
    virtual ~PlayerMoneyResponseEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<int32_t>(credits_bank);
        buffer.write<int32_t>(credits_inventory);
    }
};

} // packets

#endif // ANH_PACKETS_PLAYER_MONEY_RESPONSE_H