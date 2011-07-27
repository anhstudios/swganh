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

#include <boost/pool/pool_alloc.hpp>

#include "anh/byte_buffer.h"
#include "anh/event_dispatcher/event_dispatcher_interface.h"

#include "anh/network/soe/packet.h"
#include "anh/network/soe/session.h"
#include "anh/network/soe/socket.h"
#include "anh/network/soe/filters/receive_packet_filter.h"
#include "anh/network/soe/filters/crc_in_filter.h"
#include "anh/network/soe/filters/decryption_filter.h"
#include "anh/network/soe/filters/decompression_filter.h"
#include "anh/network/soe/filters/soe_protocol_filter.h"
#include "anh/network/soe/filters/outgoing_start_filter.h"
#include "anh/network/soe/filters/compression_filter.h"
#include "anh/network/soe/filters/crc_out_filter.h"
#include "anh/network/soe/filters/encryption_filter.h"
#include "anh/network/soe/filters/send_packet_filter.h"

using namespace anh;
using namespace network::soe;
using namespace filters;
using namespace std;
using namespace tbb;

Server::Server(MessageHandler message_handler)
    : event_dispatcher_(nullptr)
    , crc_seed_(0xDEADBABE)
    , message_handler_(message_handler)
{}

Server::~Server(void)
{	
}

void Server::Start(uint16_t port)
{
    socket_ = make_shared<Socket>(
        io_service_, 
        port, 
        bind(&Server::OnSocketRecv_, this, placeholders::_1, placeholders::_2));
    
    incoming_filter_ = 
        make_filter<void, shared_ptr<Packet>>(filter::serial_in_order, ReceivePacketFilter(incoming_messages_)) &
        make_filter<shared_ptr<Packet>, shared_ptr<Packet>>(filter::parallel, CrcInFilter()) &
        make_filter<shared_ptr<Packet>, shared_ptr<Packet>>(filter::parallel, DecryptionFilter()) &
        make_filter<shared_ptr<Packet>, shared_ptr<Packet>>(filter::parallel, DecompressionFilter()) &
        make_filter<shared_ptr<Packet>, void>(filter::serial_in_order, SoeProtocolFilter());
        
    outgoing_filter_ = 
        make_filter<void, shared_ptr<Packet>>(filter::serial_in_order, OutgoingStartFilter(outgoing_messages_)) &
        make_filter<shared_ptr<Packet>, shared_ptr<Packet>>(filter::parallel, CompressionFilter()) &
        make_filter<shared_ptr<Packet>, shared_ptr<Packet>>(filter::parallel, EncryptionFilter()) &
        make_filter<shared_ptr<Packet>, shared_ptr<Packet>>(filter::parallel, CrcOutFilter()) &
        make_filter<shared_ptr<Packet>, void>(filter::serial_in_order, SendPacketFilter(socket_));
}

void Server::Update(void)
{
    io_service_.poll();

    parallel_pipeline(1000, incoming_filter_);
    parallel_pipeline(1000, outgoing_filter_);

    session_manager_.Update();
}

void Server::Shutdown(void) {
    socket_.reset();
}
    
void Server::SendMessage(shared_ptr<Session> session, shared_ptr<ByteBuffer> message) {    
    outgoing_messages_.push(make_shared<Packet>(session, message));
}
    
void Server::HandleMessage(shared_ptr<Session> session, shared_ptr<ByteBuffer> message) {    
    message_handler_(session, message);
}

void Server::OnSocketRecv_(boost::asio::ip::udp::endpoint remote_endpoint, std::shared_ptr<anh::ByteBuffer> message) {
    // Query the SessionManager for the Session.
    std::shared_ptr<Session> session = session_manager_.GetSession(remote_endpoint);

    // If the Session doesnt exist, check for a Session Requesst.
    if(session == nullptr) {
        session = make_shared<Session>(remote_endpoint, this);
    }
        
    incoming_messages_.push(make_shared<Packet>(session, message));
}

std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> Server::event_dispatcher() {
    return event_dispatcher_;
}

void Server::event_dispatcher(std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher) {
    event_dispatcher_ = event_dispatcher;
}

SessionManager& Server::session_manager() {
    return session_manager_;
}

std::shared_ptr<Socket> Server::socket() {
    return socket_;
}

shared_ptr<ByteBuffer> Server::AllocateBuffer() {    
    auto allocated_buffer = allocate_shared<ByteBuffer, boost::pool_allocator<ByteBuffer>>(
        boost::pool_allocator<ByteBuffer>());

    return allocated_buffer;
}
