#include <anh/network/cluster/tcp_connection.h>
#include <iostream>

namespace anh {
namespace network {
namespace cluster {
void TCPConnection::Start() {

    anh::ByteBuffer in_buffer;
    boost::asio::async_read(socket_, boost::asio::buffer(recv_buffer_), [&]
    (const boost::system::error_code& error, std::size_t bytes_transferred){
        if(bytes_transferred > 2 || !error || error == boost::asio::error::message_size)
            {
                std::cout << "async_read " << recv_buffer_.data();
                bytes_recv_ += bytes_transferred;
                callback_(endpoint_, std::make_shared<ByteBuffer>((const unsigned char*)recv_buffer_.data(), bytes_transferred));
            }
    });
}


} // cluster
} // network
} // anh