#ifndef ANH_PACKETS_CLIENT_PERMISSIONS_MESSAGE_H
#define ANH_PACKETS_CLIENT_PERMISSIONS_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on server
namespace packets {
struct ClientPermissionsMessage : public BasePacket
{
    ClientPermissionsMessage(std::shared_ptr<network::Session> session_ = nullptr, uint8_t galaxy_open_ = 0
        , uint8_t charslot_open_ = 0, uint8_t unl_char_creation_ = 0)
        : BasePacket(session_, CLIENT)
        , galaxy_open(galaxy_open_)
        , charslot_open(charslot_open_)
        , unl_char_creation(unl_char_creation_)
    {}
    uint8_t galaxy_open;
    uint8_t charslot_open;
    uint8_t unl_char_creation;
};

class ClientPermissionsMessageEvent : public anh::event_dispatcher::BasicEvent<ClientPermissionsMessage>{
public:    
    ClientPermissionsMessageEvent(std::shared_ptr<network::Session> session_ = nullptr, uint8_t galaxy_open_ = 0
        , uint8_t charslot_open_ = 0, uint8_t unl_char_creation_ = 0) 
        : anh::event_dispatcher::BasicEvent<ClientPermissionsMessage>("ClientPermissionsMessage"){}
    virtual ~ClientPermissionsMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) { 
        buffer.write<uint8_t>(galaxy_open);
        buffer.write<uint8_t>(charslot_open);
        buffer.write<uint8_t>(unl_char_creation);
    }
};

} // packets

#endif // ANH_PACKETS_CLIENT_PERMISSIONS_MESSAGE_H