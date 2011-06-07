#ifndef ANH_PACKETS_LOGINCLUSTERSTATUS_H
#define ANH_PACKETS_LOGINCLUSTERSTATUS_H

#include <anh/byte_buffer.h>
#include <algorithm>
#include <list>

// Originates on Server
// http://wiki.swganh.org/index.php/LoginClusterStatus
namespace packets {
struct ClusterServer
{
    ClusterServer(uint32_t server_id_ = 0, std::string& address_ = std::string(""), uint16_t conn_port_ = 0,
        uint16_t ping_port_ = 0, uint32_t server_pop_ = 0, uint32_t max_pop_ = 0, uint32_t max_chars_ = 0, uint32_t distance_ = 0,
        uint32_t status_ = 0, uint8_t not_recommended_flag_ = 0)
        : server_id(server_id_)
        , address(address_)
        , conn_port(conn_port_)
        , ping_port(ping_port_)
        , server_pop(server_pop_)
        , max_pop(max_pop_)
        , max_chars(max_chars_)
        , distance(distance_)
        , status(status_)
        , not_recommended_flag(not_recommended_flag_)
    {}

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
struct LoginClusterStatus
{
    LoginClusterStatus(uint32_t server_count_ = 0, 
        std::list<ClusterServer> servers_ = std::list<ClusterServer>())
        : server_count(server_count_)
        , servers(servers_) {}

    explicit LoginClusterStatus(anh::ByteBuffer buffer)
    {
        deserialize(buffer);
    }
    uint32_t server_count;
    std::list<ClusterServer> servers;

    virtual ~LoginClusterStatus() {}
    void serialize(anh::ByteBuffer& buffer) const {
        buffer.write<uint16_t>(operand_count());
        buffer.write<uint32_t>(crc());
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
    void deserialize(anh::ByteBuffer buffer) 
    {
        server_count = buffer.read<uint32_t>();
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
    static uint16_t operand_count() { return 2; }
    static uint32_t crc() { return 0x3436AEB6; }
};

} // packets

#endif // ANH_PACKETS_LOGINCLUSTERSTATUS_H