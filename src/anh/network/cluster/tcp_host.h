#ifndef ANH_NETWORK_CLUSTER_TCP_CONNECTION_H
#define ANH_NETWORK_CLUSTER_TCP_CONNECTION_H

#include <anh/memory.h>
#include <anh/byte_buffer.h>
#include <boost/asio.hpp>
namespace anh {
namespace network {
namespace cluster {

using boost::asio::ip::tcp;

class tcp_host : public std::enable_shared_from_this<tcp_host> {
public:
    tcp_host(boost::asio::io_service& io_service)
    : socket_(io_service) 
    {
    }
    tcp_host::~tcp_host();
    tcp::socket& socket() { return socket_; }
    /**
    * @brief starts the connection
    */
    void Start();

    /**
    *  @brief
    */
    // broadcast to all servers

    // broadcast to server group

    // send message to a single server
    
private:
    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];

};


} // cluster
} // network
} // anh
#endif // ANH_NETWORK_CLUSTER_TCP_CONNECTION_H