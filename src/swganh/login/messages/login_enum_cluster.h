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

#ifndef SWGANH_LOGIN_MESSAGES_LOGIN_ENUM_CLUSTER_H_
#define SWGANH_LOGIN_MESSAGES_LOGIN_ENUM_CLUSTER_H_

#include <cstdint>
#include <algorithm>
#include <list>
#include <memory>
#include <string>
#include <vector>

#include "anh/byte_buffer.h"

#include "swganh/base/swg_message.h"

#include "swganh/login/galaxy_status.h"

namespace swganh {
namespace login {
struct LoginClient;
}}  // namespace swganh::login

namespace swganh {
namespace login {
namespace messages {

struct Cluster {
    uint32_t server_id;
    std::string server_name;
    // D = 3600 * GMTOffset
    uint32_t distance;
};

struct LoginEnumCluster : public swganh::base::SwgMessage<LoginEnumCluster> {
    static const uint16_t opcount() { return 3; }
    static const uint32_t opcode() { return 0xC11C63B9; }

    std::list<Cluster> servers;
    uint32_t max_account_chars;
    
    void onSerialize(anh::ByteBuffer& buffer) const {
        buffer.write<uint32_t>(servers.size());
        std::for_each(servers.begin(), servers.end(), [&buffer] (Cluster cluster){
            buffer.write<int32_t>(cluster.server_id);
            buffer.write<std::string>(cluster.server_name);
            buffer.write<uint32_t>(cluster.distance);
        });
        buffer.write<uint32_t>(max_account_chars);
    }

    void onDeserialize(anh::ByteBuffer buffer) {
        uint32_t server_count = buffer.read<uint32_t>();
        for(uint32_t i = 0; i < server_count; i++)
        {
            Cluster cluster;
            cluster.server_id = buffer.read<int32_t>();
            cluster.server_name = buffer.read<std::string>();
            cluster.distance = buffer.read<int32_t>();
            servers.push_back(cluster);
        }
        max_account_chars = buffer.read<uint32_t>();
    }
};

LoginEnumCluster BuildLoginEnumCluster(std::shared_ptr<swganh::login::LoginClient> login_client, const std::vector<login::GalaxyStatus>& galaxy_status);

}}}  // namespace swganh::login::messages

#endif  // SWGANH_LOGIN_MESSAGES_LOGIN_ENUM_CLUSTER_H_
