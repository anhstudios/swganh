#ifndef ANH_CHARACTER_SHEET_RESPONSE_MESSAGE_H
#define ANH_CHARACTER_SHEET_RESPONSE_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server [ S -> C]
// http://wiki.swganh.org/index.php/CharacterSheetResponseMessage
namespace packets {
struct CharacterSheetResponseMessage : public BasePacket
{
    CharacterSheetResponseMessage(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, float bind_x_= 0.0f, float bind_y_= 0.0f, float bind_z_ = 0.0f,
        std::string& bind_planet_ = std::string(), float bank_x_ = 0.0f, float bank_y_ = 0.0f, float bank_z_ = 0.0f, std::string& bank_planet_ = std::string(), float home_loc_x_ = 0.0f,
        float home_loc_y_ = 0.0f, float home_loc_z_ = 0.0f, std::string& home_planet_ = std::string(), std::wstring& spouse_name_ = std::wstring(),
        uint32_t lots_remaining_ = 0, int32_t faction_crc_ = 0, int32_t faction_status_ = 0)
        : BasePacket(session_, CLIENT)
        , not_used1(0)
        , not_used2(0)
        , bind_x(bind_x_)
        , bind_y(bind_y_)
        , bind_z(bind_z_)
        , bind_planet(bind_planet_)
        , bank_x(bank_x_)
        , bank_y(bank_y_)
        , bank_z(bank_z_)
        , bank_planet(bank_planet_)
        , home_loc_x(home_loc_x_)
        , home_loc_y(home_loc_y_)
        , home_loc_z(home_loc_z_)
        , home_planet(home_planet_)
        , spouse_name(spouse_name_)
        , lots_remaining(lots_remaining_)
        , faction_crc(faction_crc_)
        , faction_status(faction_status_)
        {}
    int32_t not_used1;
    int32_t not_used2;
    float bind_x;
    float bind_y;
    float bind_z;
    std::string bind_planet;
    float bank_x;
    float bank_y;
    float bank_z;
    std::string bank_planet;
    float home_loc_x;
    float home_loc_y;
    float home_loc_z;
    std::string home_planet;
    std::wstring spouse_name;
    uint32_t lots_remaining;
    int32_t faction_crc;
    int32_t faction_status;
};

class CharacterSheetResponseMessageEvent : public anh::event_dispatcher::BasicEvent<CharacterSheetResponseMessage>{
public:    
    CharacterSheetResponseMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, float bind_x_= 0.0f, float bind_y_= 0.0f, float bind_z_ = 0.0f,
        std::string& bind_planet_ = std::string(), float bank_x_ = 0.0f, float bank_y_ = 0.0f, float bank_z_ = 0.0f, std::string& bank_planet_ = std::string(), float home_loc_x_ = 0.0f,
        float home_loc_y_ = 0.0f, float home_loc_z_ = 0.0f, std::string& home_planet_ = std::string(), std::wstring& spouse_name_ = std::wstring(),
        uint32_t lots_remaining_ = 0, int32_t faction_crc_ = 0, int32_t faction_status_ = 0) 
        : anh::event_dispatcher::BasicEvent<CharacterSheetResponseMessage>("CharacterSheetResponseMessage"){}
    virtual ~CharacterSheetResponseMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer)
    {
        buffer.write<int32_t>(not_used1);
        buffer.write<int32_t>(not_used2);
        buffer.write<float>(bind_x);
        buffer.write<float>(bind_y);
        buffer.write<float>(bind_z);
        buffer.write<std::string>(bind_planet);
        buffer.write<float>(bank_x);
        buffer.write<float>(bank_y);
        buffer.write<float>(bank_z);
        buffer.write<std::string>(bank_planet);
        buffer.write<float>(home_loc_x);
        buffer.write<float>(home_loc_y);
        buffer.write<float>(home_loc_z);
        buffer.write<std::string>(home_planet);
        buffer.write<std::wstring>(spouse_name);
        buffer.write<uint32_t>(lots_remaining);
        buffer.write<int32_t>(faction_crc);
        buffer.write<int32_t>(faction_status);
    }
};

} // packets

#endif // ANH_CHARACTER_SHEET_RESPONSE_MESSAGE_H