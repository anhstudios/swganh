// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_LOGIN_CLIENT_ID_H_
#define SWGANH_MESSAGES_LOGIN_CLIENT_ID_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct LoginClientId : public BaseSwgMessage<LoginClientId>
    {
    	static uint16_t Opcount() { return 4; }
    	static uint32_t Opcode() { return 0x41131F96; }
    
    	std::string username;
    	std::string password;
    	std::string client_version; // <year><month><day>-<hour>:<minute> for publish, e.g. Publish 13.0 = 20050125-12:19
    
    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(username);
    		buffer.write(password);
    		buffer.write(client_version);
    	}
    
    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		username = buffer.read<std::string>();
    		password = buffer.read<std::string>();
    		client_version = buffer.read<std::string>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_LOGIN_CLIENT_ID_H_
