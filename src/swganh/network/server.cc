// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "server.h"

#include "swganh/logger.h"

#include "swganh/byte_buffer.h"

#include "swganh/network/session.h"

using namespace swganh::network;
using boost::asio::ip::udp;
using boost::asio::buffer;
using swganh::ByteBuffer;

Server::Server(boost::asio::io_service& io_service)
    : socket_(io_service)
    , max_receive_size_(496)
{}

Server::~Server() {}

void Server::StartListening(uint16_t port)
{
    listen_endpoint_ = udp::endpoint(udp::v4(), port);
    socket_.open(udp::v4());
    socket_.bind(listen_endpoint_);
    
    AsyncReceive();
}

void Server::StopListening()
{
	socket_.close();
}

void Server::SendTo(const udp::endpoint& endpoint, ByteBuffer buffer) {
    socket_.async_send_to(boost::asio::buffer(buffer.data(), buffer.size()), 
        endpoint, 
        [this] (const boost::system::error_code& error, std::size_t bytes_transferred)
    {
        bytes_sent_ += bytes_transferred;
    });
}

void Server::AsyncReceive() {
    socket_.async_receive_from(
        buffer(recv_buffer_.data(), recv_buffer_.size()),
        current_remote_endpoint_,
        [this] (const boost::system::error_code& error, std::size_t bytes_transferred)
    {
        if(!error && bytes_transferred > 0)
        {
            bytes_recv_ += bytes_transferred;

            GetSession(current_remote_endpoint_)->HandleProtocolMessage(ByteBuffer(recv_buffer_.data(), bytes_transferred));                			
        }

        if(error != boost::asio::error::operation_aborted) {
            AsyncReceive();
        }
    });
}

uint32_t Server::max_receive_size() {
    return max_receive_size_;
}
