#include <anh/network/cluster/tcp_connection.h>
#include <iostream>

namespace anh {
namespace network {
namespace cluster {
void TCPConnection::Start() {

    anh::ByteBuffer in_buffer;
    boost::asio::async_read(socket_, boost::asio::buffer(recv_buffer_), [&]
    (const boost::system::error_code& error, std::size_t bytes_transferred){
        std::cout << "async_read";
        
        // do input stuff
    });
}


} // cluster
} // network
} // anh