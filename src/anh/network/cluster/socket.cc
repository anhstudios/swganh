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
 
#include "anh/network/cluster/socket.h"

#include <glog/logging.h>

using namespace anh::network::cluster;
using namespace boost::asio;
using namespace boost::system;
using namespace std;
using ip::tcp;

Socket::Socket(io_service& service, const std::string& host, uint16_t port, NetworkCallback callback)
    : socket_(service)
    , resolver_(service)
    , bytes_recv_(0)
    , bytes_sent_(0)
{
    current_remote_endpoint_ = endpoint(ip::address::from_string(host), port);
    socket_.async_connect(
        current_remote_endpoint_, 
        [this] (const boost::system::error_code& error)
    {
        if(!error) {
            DLOG(WARNING) << "Connection To Endpoint " << current_remote_endpoint_.address() << ":" << current_remote_endpoint_.port()
                << " Successful..." << std::endl;
        } else {
            LOG(WARNING) << "Error in tcp_client::handle_connect_: " << error.message() << std::endl;
        }
    });
}
    
Socket::~Socket() {

}

void Socket::Send(const endpoint& remote_endpoint, anh::ByteBuffer message) {    
    socket_.async_write_some(buffer(message.data(), message.size()),
        [this] (const boost::system::error_code& error, std::size_t bytes_transferred)
    {
        bytes_sent_ += bytes_transferred;
        DLOG(WARNING) << "Send of " << bytes_transferred << " bytes successful..." << std::endl;
    });
}

void Socket::StartRead() {    
    socket_.set_option(tcp::no_delay(true));

    socket_.async_read_some(
        boost::asio::buffer(&recv_buffer_[0], recv_buffer_.size()),
        [this] (const boost::system::error_code& error, size_t bytes_transferred) 
    {
        if (!error) {
            auto buffer = make_shared<ByteBuffer>(recv_buffer_.data(), bytes_transferred);
            DLOG(WARNING) << "message received: " << buffer->size();
            callback_(current_remote_endpoint_, buffer);
            StartRead();
        } else {
            LOG(WARNING) << "Error in async_read_some: " << error.message() << std::endl;
        }
    });
}
