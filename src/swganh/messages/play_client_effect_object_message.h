// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_PLAY_CLIENT_EFFECT_OBJECT_MESSAGE_H_
#define SWGANH_MESSAGES_PLAY_CLIENT_EFFECT_OBJECT_MESSAGE_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct PlayClientEffectObjectMessage : public BaseSwgMessage<PlayClientEffectObjectMessage>
    {
    	static uint16_t Opcount() { return 3; }
    	static uint32_t Opcode() { return 0x8855434A; }

    	std::string client_effect_file; // e.g. "clienteffect/frs_dark_envy.cef"
    	std::string auxiliary_string; // Place from where to start the animation. See wiki for examples.
    	uint64_t object_id;

    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(client_effect_file);
    		buffer.write(auxiliary_string);
    		buffer.write(object_id);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		client_effect_file = buffer.read<std::string>();
    		auxiliary_string = buffer.read<std::string>();
    		object_id = buffer.read<uint64_t>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_PLAY_CLIENT_EFFECT_OBJECT_MESSAGE_H_
