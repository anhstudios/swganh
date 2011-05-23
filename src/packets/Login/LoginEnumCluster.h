#ifndef ANH_PACKETS_LOGINENUMCLUSTER_H
#define ANH_PACKETS_LOGINENUMCLUSTER_H

#include <packets/BasePacket.h>
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
struct LoginEnumCluster : public BasePacket
{
    LoginEnumCluster(uint32_t max_account_chars_ = 0,
        std::list<Cluster> servers_ = std::list<Cluster>(), std::shared_ptr<anh::network::soe::Session> session_  = nullptr)
        : BasePacket(session_, CLIENT) 
        , servers(servers_)
        , max_account_chars(max_account_chars_)
    {}
    std::list<Cluster> servers;
    uint32_t max_account_chars;
};

class LoginEnumClusterEvent : public anh::event_dispatcher::BasicEvent<LoginEnumCluster>{
public:    
    LoginEnumClusterEvent(uint32_t max_account_chars_ = 0,
        std::list<Cluster> servers_ = std::list<Cluster>(), std::shared_ptr<anh::network::soe::Session> session_  = nullptr) 
        : anh::event_dispatcher::BasicEvent<LoginEnumCluster>("LoginEnumCluster"){}
    virtual ~LoginEnumClusterEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
		buffer.write<uint16_t>(3);
		buffer.write<uint32_t>(0xC11C63B9);
        buffer.write<uint32_t>(servers.size());
        std::for_each(servers.begin(), servers.end(), [&buffer] (Cluster cluster){
            buffer.write<int32_t>(cluster.server_id);
            buffer.write<std::string>(cluster.server_name);
            buffer.write<int32_t>(cluster.distance);
        });
        buffer.write<uint32_t>(max_account_chars);
    }
};

} // packets

#endif // ANH_PACKETS_LOGINENUMCLUSTER_H