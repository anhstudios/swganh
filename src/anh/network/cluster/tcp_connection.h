#ifndef ANH_NETWORK_CLUSTER_TCP_CONNECTION_H
#define ANH_NETWORK_CLUSTER_TCP_CONNECTION_H

#include <anh/memory.h>
#include <anh/byte_buffer.h>
#include <boost/asio.hpp>
namespace anh {
namespace network {
namespace cluster {

using boost::asio::ip::tcp;
typedef std::function<void(tcp::endpoint&, std::shared_ptr<anh::ByteBuffer>)> ConnectionCallback;

class TCPConnection : public std::enable_shared_from_this<TCPConnection> {
public:
    TCPConnection(boost::asio::io_service& io_service, ConnectionCallback callback)
    : socket_(io_service) 
    , callback_(callback)
    {
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
    void HandleWrite_();
    ConnectionCallback                  callback_;
    tcp::socket socket_;
    tcp::endpoint endpoint_;
    boost::array<char, 456>				recv_buffer_;
    uint32_t                            bytes_recv_ ;

};


} // cluster
} // network
} // anh
#endif // ANH_NETWORK_CLUSTER_TCP_CONNECTION_H