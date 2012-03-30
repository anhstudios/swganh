/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

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
    : io_service_(io_service)
    , strand_(io_service)
    , socket_(io_service)
    , max_receive_size_(496)
{}

Server::~Server(void)
{	
}

void Server::Start(uint16_t port)
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
    udp::resolver resolver(io_service_);
    udp::resolver::query query(udp::v4(), hostname, "");
    udp::endpoint resolved_endpoint = *resolver.resolve(query);

    return resolved_endpoint.address().to_string();
}

void Server::AsyncReceive() {
    socket_.async_receive_from(
        buffer(&recv_buffer_[0], recv_buffer_.size()), 
        current_remote_endpoint_,
        [this] (const boost::system::error_code& error, std::size_t bytes_transferred) {
            if(bytes_transferred > 2 || !error || error == boost::asio::error::message_size)
            {
                bytes_recv_ += bytes_transferred;

                ByteBuffer message;
                message.write((const unsigned char*)recv_buffer_.data(), bytes_transferred);

                GetSession(current_remote_endpoint_)->HandleProtocolMessage(move(message));
            }

            AsyncReceive();
    });
}

boost::asio::ip::udp::socket* Server::socket() {
    return &socket_;
}

uint32_t Server::max_receive_size() {
    return max_receive_size_;
}

shared_ptr<ByteBuffer> Server::AllocateBuffer() {    
    auto allocated_buffer = std::allocate_shared<ByteBuffer, boost::pool_allocator<ByteBuffer>>(
        boost::pool_allocator<ByteBuffer>());

    return allocated_buffer;
}
