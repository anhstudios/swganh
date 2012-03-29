
#ifndef SWGANH_MESSAGES_CONTROLLERS_ANIMATE_H_
#define SWGANH_MESSAGES_CONTROLLERS_ANIMATE_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"

namespace swganh {
namespace messages {
namespace controllers {

    class Animate
    {
    public:
        Animate(const std::string& animation_, uint64_t object_id_)
            : animation(animation_)
            , object_id(object_id_)
        {}
        static uint32_t message_type() { return 0x000000F2; }
        
        uint64_t object_id;
        std::string animation;

        void Serialize(anh::ByteBuffer& buffer) const
        {
            buffer.write(object_id);
            buffer.write<uint32_t>(0);
            buffer.write(animation);
        }

        void Deserialize(anh::ByteBuffer buffer)
        {
            object_id = buffer.read<uint64_t>();
            buffer.read<uint32_t>();
            animation = buffer.read<std::string>();
        }
    };

}}}  // namespace swganh::messages::controllers


#endif  // SWGANH_MESSAGES_CONTROLLERS_SHOW_FLY_TEXT_H_
