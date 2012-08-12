// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "anh/network/soe/server.h"

#include "anh/logger.h"
#include <boost/pool/pool_alloc.hpp>

#include "anh/byte_buffer.h"

#include "anh/network/soe/session.h"

using namespace anh;
using namespace network::soe;
using namespace filters;
using namespace std;
using boost::asio::ip::udp;
using boost::asio::buffer;

Server::Server(boost::asio::io_service& io_service)
    : socket_(io_service)
    , max_receive_size_(496)
{}

Server::~Server(void)
{	
}

void Server::Startup(uint16_t port)
{
    socket_.open(udp::v4());
    socket_.bind(udp::endpoint(udp::v4(), port));
    
    AsyncReceive();
}

void Server::Shutdown(void) {
    socket_.close();
}
    
void Server::SendTo(const udp::endpoint& endpoint, ByteBuffer buffer) {
    socket_.async_send_to(boost::asio::buffer(buffer.data(), buffer.size()), 
        endpoint, 
        [this] (const boost::system::error_code& error, std::size_t bytes_transferred)
    {
        if (bytes_transferred == 0)
        {
            LOG(warning) << "Sent 0 bytes";
        }
		
        bytes_sent_ += bytes_transferred;
    });
}

string Server::Resolve(const string& hostname)
{
    udp::resolver resolver(socket_.get_io_service());
    udp::resolver::query query(udp::v4(), hostname, "");
    udp::endpoint resolved_endpoint = *resolver.resolve(query);

    return resolved_endpoint.address().to_string();
}

void Server::AsyncReceive() {
    socket_.async_receive_from(
        buffer(&recv_buffer_[0], recv_buffer_.size()), 
        current_remote_endpoint_,
        [this] (const boost::system::error_code& error, std::size_t bytes_transferred) {
            if(!error || error == boost::asio::error::message_size)
            {
                bytes_recv_ += bytes_transferred;

                ByteBuffer message(bytes_transferred);
				message.write((const unsigned char*)recv_buffer_.data(), bytes_transferred);

                GetSession(current_remote_endpoint_)->HandleProtocolMessage(move(message));

                AsyncReceive();
            }
			else
			{
				LOG(info) << "Server.cc Error in AsyncReceive: " << error.message();
				AsyncReceive();
			}
    });
}

boost::asio::ip::udp::socket* Server::socket() {
    return &socket_;
}

uint32_t Server::max_receive_size() {
    return max_receive_size_;
}
