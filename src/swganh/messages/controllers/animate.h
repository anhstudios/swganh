
#ifndef SWGANH_MESSAGES_CONTROLLERS_ANIMATE_H_
#define SWGANH_MESSAGES_CONTROLLERS_ANIMATE_H_

#include <string>
#include "anh/byte_buffer.h"

namespace swganh {
namespace messages {
namespace controllers {

    class Animate
    {
    public:
        Animate(const std::string& animation_)
            : animation(animation_){}
        static uint32_t header() { return 0x000000F2; }
        
        std::string animation;

        void Serialize(anh::ByteBuffer& buffer) const
        {
            buffer.write(animation);
        }

        void Deserialize(anh::ByteBuffer buffer)
        {
            animation = buffer.read<std::string>();
        }
    };

}}}  // namespace swganh::messages::controllers


#endif  // SWGANH_MESSAGES_CONTROLLERS_SHOW_FLY_TEXT_H_
