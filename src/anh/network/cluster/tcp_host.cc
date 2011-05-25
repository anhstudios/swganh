#include <anh/network/cluster/tcp_host.h>
#include <boost/bind.hpp>
#include <iostream>

namespace anh {
namespace network {
namespace cluster {
tcp_host::~tcp_host()
{
    socket_.close();
}
void tcp_host::Start() {
    socket_.async_read_some(boost::asio::buffer(data_, max_length),[&] 
    (const boost::system::error_code& error,size_t bytes_transferred) {
        if (!error)
        {
            auto buffer = std::make_shared<anh::ByteBuffer>((const unsigned char*)data_, bytes_transferred);
            std::cout << "trigger event";
            // create event and trigger
        }
        else
        {
            std::cout << "Error: " << error.message() << std::endl;
        }
    });
}

} // cluster
} // network
} // anh