// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <string>
#include "swganh/byte_buffer.h"

#include "swganh_core/messages/obj_controller_message.h"

namespace swganh {
namespace messages {
namespace controllers {

    class ChangeTarget : public ObjControllerMessage
    {
    public:
        explicit ChangeTarget(uint32_t controller_type = 0x00000083)
            : ObjControllerMessage(controller_type, message_type())
        {}

        ChangeTarget(const uint64_t& target_id)
            : target_id_(target_id)
        {}

		ChangeTarget(const ObjControllerMessage& base)
			: ObjControllerMessage(base)
		{
		}

        static uint32_t message_type() { return 0x00000126; }
        
		uint64_t target_id_;

        void OnControllerSerialize(swganh::ByteBuffer& buffer) const
        {
			buffer.write(target_id_);
        }

        void OnControllerDeserialize(swganh::ByteBuffer& buffer)
        {
			target_id_ = buffer.read<uint64_t>();
        }
    };

}}}  // namespace swganh::messages::controllers
