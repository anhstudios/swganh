#ifndef ANH_PACKETS_BADGES_RESPONSE_MESSAGE_H
#define ANH_PACKETS_BADGES_RESPONSE_MESSAGE_H

#include <packets/BasePacket.h>
#include <list>
#include <algorithm>

// Originates on Server
// http://wiki.swganh.org/index.php/BadgesResponseMessage
namespace packets {
struct BadgesResponseMessage : public BasePacket
{
    BadgesResponseMessage(std::shared_ptr<network::Session> session_= nullptr, uint64_t character_id_ = 0, 
        uint32_t bit_mask_count_ = 0, std::list<int32_t> bitmask_ = std::list<int32_t>())
        : BasePacket(session_, CLIENT)
        , character_id(character_id_)
        , bit_mask_count(bit_mask_count_)
        , bitmask(bitmask_)
    {}
    uint64_t character_id;
    uint32_t bit_mask_count;
    std::list<int32_t> bitmask;
};

class BadgesResponseMessageEvent : public anh::event_dispatcher::BasicEvent<BadgesResponseMessage>{
public:    
    BadgesResponseMessageEvent(std::shared_ptr<network::Session> session_= nullptr, uint64_t character_id_ = 0, 
        uint32_t bit_mask_count_ = 0, std::list<int32_t> bitmask_ = std::list<int32_t>()) 
        : anh::event_dispatcher::BasicEvent<BadgesResponseMessage>("BadgesResponseMessage"){}
    virtual ~BadgesResponseMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint64_t>(character_id);
        buffer.write<uint32_t>(bitmask.size());
        // loop through each bit_mask
        std::for_each(bitmask.begin(), bitmask.end(), [&buffer] (int32_t bit_mask) {
            buffer.write<int32_t>(bit_mask);
        });
};

} // packets

#endif // ANH_PACKETS_BADGES_RESPONSE_MESSAGE_H