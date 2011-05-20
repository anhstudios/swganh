#include <anh/network/cluster/tcp_connection.h>

namespace anh {
namespace network {
namespace cluster {
void TCPConnection::Start() {

    anh::ByteBuffer in_buffer;
    boost::asio::async_read(socket_, boost::asio::buffer(in_buffer.data(), 1024), [](){
        // do input stuff
    });
}


} // cluster
} // network
} // anh