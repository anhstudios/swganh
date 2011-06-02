#ifndef ANH_NETWORK_CLUSTER_TCP_CONNECTION_H
#define ANH_NETWORK_CLUSTER_TCP_CONNECTION_H

#include <anh/memory.h>
#include <anh/byte_buffer.h>
#include <boost/asio.hpp>
namespace anh {
namespace network {
namespace cluster {

using boost::asio::ip::tcp;

typedef std::function<void(std::shared_ptr<anh::ByteBuffer>)> NetworkCallback;

class tcp_host : public std::enable_shared_from_this<tcp_host> {
public:
    tcp_host(boost::asio::io_service& io_service, NetworkCallback callback)
    : socket_(io_service) 
    , callback_(callback)
    {
    }
    tcp_host::~tcp_host();
    tcp::socket& socket() { return socket_; }
    /**
    * @brief starts the TCP Host connection
    */
    void Start();

    /**
    *  @brief
    */
    
private:
    uint64_t	bytes_recv_;
    uint64_t	bytes_sent_;

    tcp::socket socket_;
    NetworkCallback callback_;

    enum { max_length = 1024 };
    char data_[max_length];

};


} // cluster
} // network
} // anh
#endif // ANH_NETWORK_CLUSTER_TCP_CONNECTION_H