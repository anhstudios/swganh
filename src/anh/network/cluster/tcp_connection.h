#ifndef ANH_NETWORK_CLUSTER_TCP_CONNECTION_H
#define ANH_NETWORK_CLUSTER_TCP_CONNECTION_H

#include <anh/memory.h>
#include <anh/byte_buffer.h>
#include <boost/asio.hpp>
namespace anh {
namespace network {
namespace cluster {

using boost::asio::ip::tcp;

class TCPConnection : public std::enable_shared_from_this<TCPConnection> {
public:
    static std::shared_ptr<TCPConnection> create(boost::asio::io_service& io_service)
    {
        return std::shared_ptr<TCPConnection>(new TCPConnection(io_service));
    }
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
    /**
    *  @brief private ctor to ensure we always get a shared_ptr
    */
    TCPConnection(boost::asio::io_service& io_service)
        : socket_(io_service) 
    {
    }
    void HandleWrite_();
    tcp::socket socket_;
};


} // cluster
} // network
} // anh
#endif // ANH_NETWORK_CLUSTER_TCP_CONNECTION_H