// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_LOGIN_CLIENT_TOKEN_H_
#define SWGANH_MESSAGES_LOGIN_CLIENT_TOKEN_H_

#include <cstdint>
#include <memory>
#include <string>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace login {
	class LoginClient;
}} // namespace swganh::login

namespace swganh {
namespace messages {

    struct LoginClientToken : public BaseSwgMessage<LoginClientToken>
    {
    	static uint16_t Opcount() { return 4; }
    	static uint32_t Opcode() { return 0xAAB296C6; }

    	anh::ByteBuffer session_key;
    	uint32_t station_id;
    	std::string station_username;

    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write<uint32_t>(session_key.size());
    		buffer.append(session_key);
    		buffer.write(station_id);
    		buffer.write(station_username);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		session_key = anh::ByteBuffer(buffer.data(), buffer.read<uint32_t>());
    		buffer.read_position(buffer.read_position() + session_key.size());
    		station_id = buffer.read<int32_t>();
    		station_username = buffer.read<std::string>();
    	}
    };

    LoginClientToken BuildLoginClientToken(std::shared_ptr<swganh::login::LoginClient> login_client, const std::string& session_key);

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_LOGIN_CLIENT_TOKEN_H_
