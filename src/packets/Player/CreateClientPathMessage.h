#ifndef ANH_PACKETS_CREATE_CLIENT_PATH_MESSAGE_H
#define ANH_PACKETS_CREATE_CLIENT_PATH_MESSAGE_H

#include <packets/BasePacket.h>
#include <list>
#include <algorithm>

// Originates on Server [ S -> C ]
namespace packets {
struct coordinates
{
    float x, y, z;
};
struct CreateClientPathMessage : public BasePacket
{
    explicit CreateClientPathMessage(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint32_t coordinates_count_ = 0,
        std::list<coordinates> coordinates_list_ = std::list<coordinates>())
    : BasePacket(session_, CLIENT)
    , coordinates_count(coordinates_count_)
    , coordinates_list(coordinates_list_)
    {}
    uint32_t coordinates_count;
    std::list<coordinates> coordinates_list;
};

class CreateClientPathMessageEvent : public anh::event_dispatcher::BasicEvent<CreateClientPathMessage>{
public:    
    CreateClientPathMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint32_t coordinates_count_ = 0,
        std::list<coordinates> coordinates_list_ = std::list<coordinates>()) 
        : anh::event_dispatcher::BasicEvent<CreateClientPathMessage>("CreateClientPathMessage"){}
    virtual ~CreateClientPathMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint32_t>(coordinates_list.size());
        std::for_each(coordinates_list.begin(), coordinates_list.end(), [&buffer] (coordinates coords) {
            buffer.write<float>(coords.x);
            buffer.write<float>(coords.y);
            buffer.write<float>(coords.z);
        });
    }
};

} // packets

#endif // ANH_PACKETS_CREATE_CLIENT_PATH_MESSAGE_H