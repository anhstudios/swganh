#ifndef ANH_PACKETS_LOGINCLUSTERSTATUS_H
#define ANH_PACKETS_LOGINCLUSTERSTATUS_H

#include <packets/BasePacket.h>
#include <algorithm>
#include <list>

// Originates on Server
// http://wiki.swganh.org/index.php/LoginClusterStatus
namespace packets {
struct ClusterServer
{
    ClusterServer(uint32_t server_id_, std::string& address_, uint16_t conn_port_,
        uint16_t ping_port_, uint32_t server_pop_, uint32_t max_pop_, uint32_t max_chars_, uint32_t distance_,
        uint32_t status_, uint8_t not_recommended_flag_)
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
struct LoginClusterStatus : public BasePacket
{
    LoginClusterStatus(std::shared_ptr<network::Session> session_  = nullptr,
        std::list<ClusterServer> servers_ = std::list<ClusterServer>())
        : BasePacket(session_, CLIENT)
        , servers(servers_) {}
    std::list<ClusterServer> servers;

    virtual ~LoginClusterStatus() {}
    
    void serialize(anh::ByteBuffer& buffer) const {
		buffer.write<uint16_t>(2);
		buffer.write<uint32_t>(0x3436AEB6);
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

    void deserialize(anh::ByteBuffer buffer) {}
};

typedef anh::event_dispatcher::BasicEvent<LoginClusterStatus> LoginClusterStatusEvent;

} // packets

#endif // ANH_PACKETS_LOGINCLUSTERSTATUS_H