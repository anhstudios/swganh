#ifndef ANH_PACKETS_ATTRIBUTE_LIST_MESSAGE_H
#define ANH_PACKETS_ATTRIBUTE_LIST_MESSAGE_H

#include <packets/BasePacket.h>
#include <list>
#include <algorithm>

// Originates on Server
// http://wiki.swganh.org/index.php/AttributeListMessage
namespace packets {
struct Attributes
{
    std::string attribute_name;
    std::wstring attribute_value;
};
struct AttributeListMessage : public BasePacket
{
    AttributeListMessage(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint64_t object_id_ = 0, 
        uint32_t list_count_ = 0, std::list<Attributes> attributes_ = std::list<Attributes>())
        : BasePacket(session_, CLIENT)
        , object_id(object_id_)
        , list_count(list_count_)
        , attributes(attributes_)
    {}
    uint64_t object_id;
    uint32_t list_count;
    std::list<Attributes> attributes;
};

class AttributeListMessageEvent : public anh::event_dispatcher::BasicEvent<AttributeListMessage>{
public:    
    AttributeListMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint64_t object_id_ = 0, 
        uint32_t list_count_ = 0, std::list<Attributes> attributes_ = std::list<Attributes>()) 
        : anh::event_dispatcher::BasicEvent<AttributeListMessage>("AttributeListMessage"){}
    virtual ~AttributeListMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint64_t>(object_id);
        buffer.write<uint32_t>(attributes.size());
        // loop through each attributes list
        std::for_each(attributes.begin(), attributes.end(), [&buffer] (Attributes attribute) {
            buffer.write<std::string>(attribute.attribute_name);
            buffer.write<std::wstring>(attribute.attribute_value);
        });
    }
};

} // packets

#endif // ANH_PACKETS_ATTRIBUTE_LIST_MESSAGE_H