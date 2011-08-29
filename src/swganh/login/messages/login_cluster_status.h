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

#ifndef SWGANH_LOGIN_MESSAGES_LOGIN_CLUSTER_STATUS_H_
#define SWGANH_LOGIN_MESSAGES_LOGIN_CLUSTER_STATUS_H_

#include <cstdint>
#include <algorithm>
#include <list>
#include <string>
#include <vector>

#include "anh/byte_buffer.h"
#include "swganh/base/swg_message.h"

#include "swganh/login/galaxy_status.h"

namespace swganh {
namespace login {
namespace messages {

struct ClusterServer {
    uint32_t server_id;
    std::string address;
    uint16_t conn_port;
    uint16_t ping_port;
    uint32_t server_pop;
    uint32_t max_pop;
    uint32_t max_chars;
    uint32_t distance;
    uint32_t status;
    uint8_t not_recommended_flag;
};

struct LoginClusterStatus : public swganh::base::SwgMessage<LoginClusterStatus> {
    static uint16_t opcount() { return 2; }
    static uint32_t opcode() { return 0x3436AEB6; }

    std::list<ClusterServer> servers;

    void onSerialize(anh::ByteBuffer& buffer) const {
        buffer.write<uint32_t>(servers.size());
        std::for_each(servers.begin(), servers.end(), [&buffer] (ClusterServer server) {
            buffer.write<uint32_t>(server.server_id);
            buffer.write<std::string>(server.address);
            buffer.write<uint16_t>(server.conn_port);
            buffer.write<uint16_t>(server.ping_port);
            buffer.write<uint32_t>(server.server_pop);
            buffer.write<uint32_t>(server.max_pop);
            buffer.write<uint32_t>(server.max_chars);
            buffer.write<uint32_t>(server.distance);
            buffer.write<uint32_t>(server.status);
            buffer.write<uint8_t>(server.not_recommended_flag);
        });
    }

    void onDeserialize(anh::ByteBuffer buffer) {        
        uint32_t server_count = buffer.read<uint32_t>();
        for(uint32_t i = 0; i < server_count; i++)
        {
            ClusterServer server;
            server.server_id = buffer.read<uint32_t>();
            server.address = buffer.read<std::string>();
            server.conn_port = buffer.read<uint16_t>();
            server.ping_port = buffer.read<uint16_t>();
            server.server_pop = buffer.read<uint32_t>();
            server.max_pop  = buffer.read<uint32_t>();
            server.distance = buffer.read<uint32_t>();
            server.status = buffer.read<uint32_t>();
            server.not_recommended_flag = buffer.read<uint8_t>();
            servers.push_back(server);
        }
    }
};

LoginClusterStatus BuildLoginClusterStatus(const std::vector<login::GalaxyStatus>& galaxy_status);

}}}  // namespace swganh::login::messages

#endif  // SWGANH_LOGIN_MESSAGES_LOGIN_CLUSTER_STATUS_H_
