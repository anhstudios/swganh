/*
 This file is part of MMOServer. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2010 The SWG:ANH Team

 MMOServer is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 MMOServer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with MMOServer.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SWGANH_LOGIN_MESSAGES_LOGIN_CLIENT_TOKEN_H_
#define SWGANH_LOGIN_MESSAGES_LOGIN_CLIENT_TOKEN_H_

#include <cstdint>
#include <memory>
#include <string>

#include "anh/byte_buffer.h"
#include "swganh/base/swg_message.h"

namespace swganh {
namespace login {
struct LoginClient;
}}  // namespace swganh::login

namespace swganh {
namespace login {
namespace messages {
    
struct LoginClientToken : public swganh::base::SwgMessage<LoginClientToken> {
    static uint16_t opcount() { return 4; }
    static uint32_t opcode() { return 0xAAB296C6; }

    anh::ByteBuffer session_key;
    uint32_t station_id;
    std::string station_username;
    
    void onSerialize(anh::ByteBuffer& buffer) const {
        buffer.write<uint32_t>(session_key.size());
        buffer.append(session_key);
        buffer.write(station_id);
        buffer.write(station_username);
    }

    void onDeserialize(anh::ByteBuffer buffer) {
        session_key = anh::ByteBuffer(buffer.data(), buffer.read<uint32_t>());
        buffer.read_position(buffer.read_position() + session_key.size());

        station_id = buffer.read<int32_t>();
        station_username = buffer.read<std::string>();
    }
};

LoginClientToken BuildLoginClientToken(std::shared_ptr<swganh::login::LoginClient> login_client, const std::string& session_key);

}}}  // namespace swganh::login::messages

#endif  // SWGANH_LOGIN_MESSAGES_LOGIN_CLIENT_TOKEN_H_
