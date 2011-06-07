#ifndef ANH_PACKETS_DATA_CHANNEL_MESSAGE_H
#define ANH_PACKETS_DATA_CHANNEL_MESSAGE_H

#include <anh/byte_buffer.h>
#include <anh/event_dispatcher/basic_event.h>

namespace packets {
struct DataChannelMessage
{
    DataChannelMessage() {}
    DataChannelMessage(uint32_t session_id_, anh::ByteBuffer data_)
    : session_id(session_id_)
    , data(data_)
    {}
    explicit DataChannelMessage(anh::ByteBuffer buffer)
    {
        serialize(buffer);
    }
    uint32_t session_id;
    uint32_t process_id;
    anh::ByteBuffer data;
    virtual ~DataChannelMessage() {}
    void serialize(anh::ByteBuffer& buffer) const
    {
    }
    void deserialize(anh::ByteBuffer buffer)
    {
    }
};

} // packets
typedef anh::event_dispatcher::BasicEvent<packets::DataChannelMessage> DataChannelEvent;

#endif // ANH_PACKETS_DATA_CHANNEL_MESSAGE_H