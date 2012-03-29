// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_LOGIN_ENUM_CLUSTER_H_
#define SWGANH_MESSAGES_LOGIN_ENUM_CLUSTER_H_

#include <cstdint>
#include <algorithm>
#include <list>
#include <memory>
#include <string>
#include <vector>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"
#include "swganh/login/galaxy_status.h"

namespace swganh {
namespace login {
	class LoginClient;
}} // namespace swganh::login

namespace swganh {
namespace messages {

    struct Cluster
    {
    	uint32_t server_id;
    	std::string server_name;
    	// D = 3600 * GMTOffset
    	uint32_t distance;
    };

    struct LoginEnumCluster : public BaseSwgMessage<LoginEnumCluster>
    {
    	static uint16_t Opcount() { return 3; }
    	static uint32_t Opcode() { return 0xC11C63B9; }

    	std::list<Cluster> servers;
    	uint32_t max_account_chars;

    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
            buffer.write<uint32_t>(servers.size());
            std::for_each(servers.begin(), servers.end(), [&buffer] (Cluster cluster){
                buffer.write<int32_t>(cluster.server_id);
                buffer.write<std::string>(cluster.server_name);
                buffer.write<uint32_t>(cluster.distance);
            });
            buffer.write<uint32_t>(max_account_chars);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
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

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_LOGIN_ENUM_CLUSTER_H_
