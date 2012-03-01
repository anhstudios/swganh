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

#include <boost/log/trivial.hpp>
#include <boost/pool/pool_alloc.hpp>

#include "anh/byte_buffer.h"
#include "anh/event_dispatcher.h"

#include "anh/network/soe/packet.h"
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
    , crc_seed_(0xDEADBABE)
    , active_(io_service)
    , max_receive_size_(496)
{}

Server::Server(boost::asio::io_service& io_service, EventDispatcher* event_dispatcher, MessageHandler message_handler)
    : io_service_(io_service)
    , strand_(io_service)
    , socket_(io_service)
    , event_dispatcher_(event_dispatcher)
    , crc_seed_(0xDEADBABE)
    , active_(io_service)
    , message_handler_(message_handler)
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

    active_.AsyncRepeated(boost::posix_time::milliseconds(5), [this] () {
        boost::lock_guard<boost::mutex> lg(session_map_mutex_);
        for_each(
            begin(session_map_), 
            end(session_map_), 
            [=] (Server::SessionMap::value_type& type) 
        {
            type.second->Update();
        });
    });
}

void Server::Shutdown(void) {
    socket_.close();
}
    
void Server::SendTo(const udp::endpoint& endpoint, const shared_ptr<ByteBuffer>& buffer) {
    socket_.async_send_to(boost::asio::buffer(buffer->data(), buffer->size()), 
        endpoint, 
        [this, buffer](const boost::system::error_code& error, std::size_t bytes_transferred)
    {
        if (bytes_transferred == 0) {
            BOOST_LOG_TRIVIAL(warning) << "Sent 0 bytes";
        }

        bytes_sent_ += bytes_transferred;
    });
}

void Server::HandleMessage(shared_ptr<Packet> packet) {    
    message_handler_(packet);
}

void Server::AsyncReceive() {
    socket_.async_receive_from(
        buffer(&recv_buffer_[0], recv_buffer_.size()), 
        current_remote_endpoint_,
        [this] (const boost::system::error_code& error, std::size_t bytes_transferred) {
            if(bytes_transferred > 2 || !error || error == boost::asio::error::message_size)
            {
                bytes_recv_ += bytes_transferred;
                OnSocketRecv_(current_remote_endpoint_, std::make_shared<ByteBuffer>((const unsigned char*)recv_buffer_.data(), bytes_transferred));
            }

            AsyncReceive();
    });
}
void Server::OnSocketRecv_(boost::asio::ip::udp::endpoint remote_endpoint, const std::shared_ptr<anh::ByteBuffer>& message) {
    strand_.post([=] () {
        auto session = GetSession(remote_endpoint);

        session->HandleProtocolMessage(message);
    });
}

bool Server::AddSession(std::shared_ptr<Session> session) {
    {
        boost::lock_guard<boost::mutex> lg(session_map_mutex_);
        if (session_map_.find(session->remote_endpoint()) == session_map_.end())
        {
            session_map_.insert(make_pair(session->remote_endpoint(), session));
        }
    }
    
    return false;
}

bool Server::RemoveSession(std::shared_ptr<Session> session) {
    {
        boost::lock_guard<boost::mutex> lg(session_map_mutex_);
        session_map_.erase(session->remote_endpoint());
    }

    return true;
}

shared_ptr<Session> Server::GetSession(const udp::endpoint& endpoint) {
    {
        boost::lock_guard<boost::mutex> lg(session_map_mutex_);

        auto find_iter = session_map_.find(endpoint);
        if (find_iter != session_map_.end())
        {
            return find_iter->second;
        }
    }

    auto session = CreateSession(endpoint);
    AddSession(session);

    return session;
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
