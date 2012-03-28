
#ifndef SWGANH_MESSAGES_CONTROLLERS_SPATIAL_CHAT_H_
#define SWGANH_MESSAGES_CONTROLLERS_SPATIAL_CHAT_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"

namespace swganh {
namespace messages {
namespace controllers {

    class SpatialChat
    {
    public:
        static uint32_t message_type() { return 0x000000F4; }
        
        uint64_t speaker_id;
        uint64_t target_id;
        std::wstring message;
        uint16_t font_size;
        uint16_t chat_type;
        uint16_t mood;
        uint8_t whisper_target_animate;
        uint8_t language;
        uint32_t prose;
        uint32_t unknown1;

        SpatialChat()
            : target_id(0)
            , font_size(0x32)
            , chat_type(0)
            , mood(0)
            , whisper_target_animate(0)
            , prose(0)
            , unknown1(0)
        {}

        void Serialize(anh::ByteBuffer& buffer) const
        {
            buffer.write(speaker_id);
            buffer.write(target_id);
            buffer.write(message);
            buffer.write(font_size);
            buffer.write(chat_type);
            buffer.write(mood);
            buffer.write(whisper_target_animate);
            buffer.write(language);
            buffer.write(prose);
            buffer.write(unknown1);
        }

        void Deserialize(anh::ByteBuffer buffer)
        {
            speaker_id = buffer.read<uint64_t>();
            target_id = buffer.read<uint64_t>();
            message = buffer.read<std::wstring>();
            font_size = buffer.read<uint16_t>();
            chat_type = buffer.read<uint16_t>();
            mood = buffer.read<uint16_t>();
            whisper_target_animate = buffer.read<uint8_t>();
            language = buffer.read<uint8_t>();
            prose = buffer.read<uint32_t>();
            unknown1 = buffer.read<uint32_t>();
        }
    };

}}}  // namespace swganh::messages::controllers

#endif  // SWGANH_MESSAGES_CONTROLLERS_SPATIAL_CHAT_H_
