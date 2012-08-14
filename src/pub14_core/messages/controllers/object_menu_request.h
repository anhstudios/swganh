// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CONTROLLERS_OBJECT_MENU_REQUEST_H_
#define SWGANH_MESSAGES_CONTROLLERS_OBJECT_MENU_REQUEST_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"

#include "pub14_core/messages/obj_controller_message.h"
#include "radial_options.h"

namespace swganh {
namespace messages {
namespace controllers {



    // Radial Request
    class ObjectMenuRequest : public ObjControllerMessage
    {
    public:
        explicit ObjectMenuRequest(uint32_t controller_type = 0x0000000B)
            : ObjControllerMessage(controller_type, message_type())
            , 
        {}

        explicit ObjectMenuRequest(ObjControllerMessage controller_message)
            : ObjControllerMessage(std::move(controller_message))
        {
            OnControllerDeserialize(std::move(data));
        }

        static uint32_t message_type() { return 0x00000146; }
        
        uint64_t item_id;
        uint64_t receiver_id;
        std::vector<RadialOptions> radial_options;
        uint8_t response_count;

        void OnControllerSerialize(anh::ByteBuffer& buffer) const
        {
            buffer.write(item_id);
            buffer.write(receiver_id);

            uint32_t size = radial_options.size();
            if (size > 0)
            {
                int counter = 0;
                for(auto& radial : radial_options)
                {
                    buffer.write(counter++);
                    buffer.write(radial.parent_item);
                    buffer.write(static_cast<uint8_t>(radial.identifier));
                    buffer.write(radial.action);
                    buffer.write(radial.custom_description);
                }
            }
            else
            {
                buffer.write(0);
            }
            buffer.write(response_count);
        }

        void OnControllerDeserialize(anh::ByteBuffer buffer)
        {
            item_id = buffer.read<uint64_t>();
            receiver_id = buffer.read<uint64_t>();
            
        }
    };

}}}  // namespace swganh::messages::controllers

#endif  // SWGANH_MESSAGES_CONTROLLERS_OBJECT_MENU_REQUEST_H_
