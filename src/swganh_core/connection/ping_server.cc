// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "ping_server.h"

#include <functional>

#include "swganh/logger.h"

#define RECEIVE_BUFFER 512

using namespace swganh::connection;

PingServer::PingServer(boost::asio::io_service& io_service, uint16_t port)
    : socket_(io_service)
    , receive_buffer_(RECEIVE_BUFFER)
{
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::udp::v4(), port);
    socket_.open(endpoint.protocol());
    socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
    socket_.bind(endpoint);

    AsyncReceive();
}

PingServer::~PingServer()
{
	socket_.close();
}

uint64_t PingServer::BytesReceived() const
{
    return bytes_received_;
}

uint64_t PingServer::BytesSent() const
{
    return bytes_sent_;
}

void PingServer::AsyncReceive()
{
    // Listen for an incoming message. When a message is received call HandleReceive.
    socket_.async_receive_from(
        boost::asio::buffer(receive_buffer_),
        remote_endpoint_,
        std::bind(
            &PingServer::HandleReceive,
            this,
            std::placeholders::_1,
            std::placeholders::_2
        )
    );
}

void PingServer::HandleReceive(const boost::system::error_code& error, size_t bytesReceived)
{
    // Check if an error occurred.
    if (!error || error == boost::asio::error::message_size) {
        bytes_received_ += bytesReceived;

        // Send the message that was just received back to the sender.
        socket_.async_send_to(
            boost::asio::buffer(&receive_buffer_[0], bytesReceived),
            remote_endpoint_,
            std::bind(
                &PingServer::HandleSend,
                this,
                std::placeholders::_1,
                std::placeholders::_2
            )
        );

        // Now that the message has been handled start listening for new requests.
        AsyncReceive();
    }
}

void PingServer::HandleSend(const boost::system::error_code& error, size_t bytesSent)
{
    bytes_sent_ += bytesSent;
}
