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
    LoginEnumCluster(std::shared_ptr<network::Session> session_  = nullptr, uint32_t server_count_ = 0, 
        std::list<Cluster> servers_ = std::list<Cluster>(), uint32_t max_account_chars_ = 0)
        : BasePacket(session_, CLIENT) 
        , server_count(server_count_)
        , servers(servers_)
        , max_account_chars(max_account_chars_)
    {}
    uint32_t server_count;
    std::list<Cluster> servers;
    uint32_t max_account_chars;
};

class LoginEnumClusterEvent : public anh::event_dispatcher::BasicEvent<LoginEnumCluster>{
public:    
    LoginEnumClusterEvent(std::shared_ptr<network::Session> session_  = nullptr, uint32_t server_count_ = 0, 
        std::list<Cluster> servers_ = std::list<Cluster>(), uint32_t max_account_chars_ = 0) 
        : anh::event_dispatcher::BasicEvent<LoginEnumCluster>("LoginEnumCluster"){}
    virtual ~LoginEnumClusterEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
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