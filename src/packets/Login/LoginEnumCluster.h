#ifndef ANH_PACKETS_LOGINENUMCLUSTER_H
#define ANH_PACKETS_LOGINENUMCLUSTER_H

#include <anh/byte_buffer.h>
#include <algorithm>
#include <list>

// Originates on Server
// http://wiki.swganh.org/index.php/LoginEnumCluster
namespace packets {
struct Cluster {
    uint32_t server_id;
    std::string server_name;
    // D = 3600 * GMTOffset
    int32_t distance;
};
struct LoginEnumCluster
{
    LoginEnumCluster(uint32_t server_count_ = 0, std::list<Cluster> servers_ = std::list<Cluster>(),
        uint32_t max_account_chars_ = 0)
        : server_count(server_count_)
        , servers(servers_)
        , max_account_chars(max_account_chars_)
    {}
    explicit LoginEnumCluster(anh::ByteBuffer buffer)
    {
        serialize(buffer);
    }

    virtual ~LoginEnumCluster() {}

    uint32_t server_count;
    std::list<Cluster> servers;
    uint32_t max_account_chars;

    void serialize(anh::ByteBuffer& buffer) const {
        buffer.write<uint16_t>(operand_count());
        buffer.write<uint32_t>(crc());
        buffer.write<uint32_t>(servers.size());
        std::for_each(servers.begin(), servers.end(), [&buffer] (Cluster cluster){
            buffer.write<int32_t>(cluster.server_id);
            buffer.write<std::string>(cluster.server_name);
            buffer.write<int32_t>(cluster.distance);
        });
        buffer.write<uint32_t>(max_account_chars);
    }
    void deserialize(anh::ByteBuffer buffer)
    {
        server_count = buffer.read<uint32_t>();
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
    static uint16_t operand_count() { return 3; }
    static uint32_t crc() { return 0xC11C63B9; }
};

} // packets

#endif // ANH_PACKETS_LOGINENUMCLUSTER_H