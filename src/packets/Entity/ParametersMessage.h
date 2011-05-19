#ifndef ANH_PACKETS_PARAMETERS_MESSAGE_H
#define ANH_PACKETS_PARAMETERS_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct ParametersMessage  : public BasePacket
{
    ParametersMessage ()
        : BasePacket() {}
    ParametersMessage (std::shared_ptr<network::Session> session_  = nullptr, int32_t parameter_flags_ = 0)
        : BasePacket(session_, CLIENT)
        , parameter_flags(parameter_flags_)
        {}
    int32_t parameter_flags;
};

class ParametersMessageEvent : public anh::event_dispatcher::BasicEvent<ParametersMessage >{
public:    
    ParametersMessageEvent(std::shared_ptr<network::Session> session_  = nullptr, int32_t parameter_flags_ = 0) 
        : anh::event_dispatcher::BasicEvent<ParametersMessage >("ParametersMessage"){}
    virtual ~ParametersMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        // doesn't seem to affect anything
        buffer.write<int32_t>(parameter_flags);
    }
};

} // packets

#endif // ANH_PACKETS_PARAMETERS_MESSAGE_H