/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef SWGANH_MESSAGES_LOGIN_CLUSTER_STATUS_H_
#define SWGANH_MESSAGES_LOGIN_CLUSTER_STATUS_H_

#include <cstdint>
#include <algorithm>
#include <list>
#include <string>
#include <vector>
#include "anh/byte_buffer.h"
#include "swganh/messages/base_swg_message.h"
#include "swganh/login/galaxy_status.h"

namespace swganh {
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

struct LoginClusterStatus : public swganh::messages::BaseSwgMessage<LoginClusterStatus> {
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

LoginClusterStatus BuildLoginClusterStatus(const std::vector<swganh::login::GalaxyStatus>& galaxy_status);

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_LOGIN_CLUSTER_STATUS_H_