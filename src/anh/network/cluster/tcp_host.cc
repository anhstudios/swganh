#include <anh/network/cluster/tcp_host.h>
#include <boost/bind.hpp>
#include <iostream>
#include <glog/logging.h>

namespace anh {
namespace network {
namespace cluster {
tcp_host::~tcp_host()
{
}
void tcp_host::Start() {
    socket_.async_read_some(boost::asio::buffer(data_, max_length),[&] 
    (const boost::system::error_code& error,size_t bytes_transferred) {
        if (!error)
        {
            auto buffer = std::make_shared<anh::ByteBuffer>((const unsigned char*)data_, bytes_transferred);
            DLOG(WARNING) << "message received: " << buffer->size();
            callback_(buffer);
            Start();
        }
        else
        {
            LOG(WARNING) << "Error in async_read_some: " << error.message() << std::endl;
        }
    });
}

} // cluster
} // network
} // anh