#ifndef ANH_PACKETS_SURVEY_MESSAGE_H
#define ANH_PACKETS_SURVEY_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct SurveyMessage  : public BasePacket
{
    SurveyMessage (std::shared_ptr<network::Session> session_  = nullptr, uint32_t survey_size_ = 0, float x_ = 0.0f
        , float y_ = 0.0f, float z_ = 0.0f, float ratio_ = 0.0f)
        : BasePacket(session_, CLIENT)
        , survey_size(survey_size_)
        , x(x_)
        , y(y_)
        , z(z_)
        , ratio(ratio_)
        {}
    uint32_t survey_size;
    float x,y,z, ratio;
};

class SurveyMessageEvent : public anh::event_dispatcher::BasicEvent<SurveyMessage>{
public:    
    SurveyMessageEvent(std::shared_ptr<network::Session> session_  = nullptr, uint32_t survey_size_ = 0, float x_ = 0.0f
        , float y_ = 0.0f, float z_ = 0.0f, float ratio_ = 0.0f) 
        : anh::event_dispatcher::BasicEvent<SurveyMessage>("SurveyMessage"){}
    virtual ~SurveyMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint32_t>(survey_size);
        buffer.write<float>(x);
        buffer.write<float>(y);
        buffer.write<float>(z);
        buffer.write<float>(ratio);
    }
};

} // packets

#endif // ANH_PACKETS_SURVEY_MESSAGE_H