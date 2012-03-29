// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_NEWBIE_TUTORIAL_RESPONSE_H_
#define SWGANH_MESSAGES_NEWBIE_TUTORIAL_RESPONSE_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct NewbieTutorialResponse : public BaseSwgMessage<NewbieTutorialResponse>
    {
    	static uint16_t Opcount() { return 2; }
    	static uint32_t Opcode() { return 0xCA88FBAD; }
    	
    	std::string client_ready; // arbitrary string: "clientReady" (lowercase c, uppercase R)
    	
    	NewbieTutorialResponse()
    		: client_ready("clientReady")
    	{}
    	
    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(client_ready);
    	}
    
    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		client_ready = buffer.read<std::string>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_NEWBIE_TUTORIAL_RESPONSE_H_
