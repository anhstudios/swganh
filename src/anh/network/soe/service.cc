/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2010 The SWG:ANH Team
---------------------------------------------------------------------------------------
Use of this source code is governed by the GPL v3 license that can be found
in the COPYING file or at http://www.gnu.org/licenses/gpl-3.0.html

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
---------------------------------------------------------------------------------------
*/

#include "anh/network/soe/service.h"

#include "anh/byte_buffer.h"
#include "anh/event_dispatcher/event_dispatcher_interface.h"

#include "anh/network/soe/incoming_packet.h"
#include "anh/network/soe/incoming_sessionless_packet.h"
#include "anh/network/soe/outgoing_packet.h"
#include "anh/network/soe/session.h"
#include "anh/network/soe/socket.h"
#include "anh/network/soe/receive_packet_filter.h"
#include "anh/network/soe/crc_in_filter.h"
#include "anh/network/soe/decryption_filter.h"
#include "anh/network/soe/decompression_filter.h"
#include "anh/network/soe/soe_protocol_filter.h"
#include "anh/network/soe/session_request_filter.h"
#include "anh/network/soe/outgoing_start_filter.h"
#include "anh/network/soe/compression_filter.h"
#include "anh/network/soe/crc_out_filter.h"
#include "anh/network/soe/encryption_filter.h"
#include "anh/network/soe/send_packet_filter.h"

using namespace anh;
using namespace network::soe;
using namespace std;
using namespace tbb;

Service::Service()
	: event_dispatcher_(nullptr)
    , crc_seed_(0xDEADBABE)
{
    sessionless_filter_ = make_filter<void, void>(filter::serial_in_order, SessionRequestFilter(sessionless_messages_));
    
    incoming_filter_ = 
        make_filter<void, shared_ptr<IncomingPacket>>(filter::serial_in_order, ReceivePacketFilter(incoming_messages_)) &
        make_filter<shared_ptr<IncomingPacket>, shared_ptr<IncomingPacket>>(filter::parallel, CrcInFilter()) &
        make_filter<shared_ptr<IncomingPacket>, shared_ptr<IncomingPacket>>(filter::parallel, DecryptionFilter()) &
        make_filter<shared_ptr<IncomingPacket>, shared_ptr<IncomingPacket>>(filter::parallel, DecompressionFilter()) &
        make_filter<shared_ptr<IncomingPacket>, void>(filter::serial_in_order, SoeProtocolFilter());
        
    outgoing_filter_ = 
        make_filter<void, shared_ptr<OutgoingPacket>>(filter::serial_in_order, OutgoingStartFilter(outgoing_messages_)) &
        make_filter<shared_ptr<OutgoingPacket>, shared_ptr<OutgoingPacket>>(filter::parallel, CompressionFilter()) &
        make_filter<shared_ptr<OutgoingPacket>, shared_ptr<OutgoingPacket>>(filter::parallel, EncryptionFilter()) &
        make_filter<shared_ptr<OutgoingPacket>, shared_ptr<OutgoingPacket>>(filter::parallel, CrcOutFilter()) &
        make_filter<shared_ptr<OutgoingPacket>, void>(filter::serial_in_order, SendPacketFilter(socket_));
}

Service::~Service(void)
{	
}

void Service::Start(uint16_t port)
{
	socket_ = make_shared<Socket>(
        io_service_, 
        port, 
        bind(&Service::OnSocketRecv_, this, placeholders::_1, placeholders::_2));
}

void Service::Update(void)
{
	io_service_.poll();

    parallel_pipeline(1000, sessionless_filter_);
    parallel_pipeline(1000, incoming_filter_);
    parallel_pipeline(1000, outgoing_filter_);

	session_manager_.Update();
}

void Service::Shutdown(void) {
	socket_.reset();
}
    
void Service::SendMessage(shared_ptr<Session> session, shared_ptr<ByteBuffer> message) {    
    outgoing_messages_.push_back(make_shared<OutgoingPacket>(session, message));
}

void Service::OnSocketRecv_(boost::asio::ip::udp::endpoint remote_endpoint, std::shared_ptr<anh::ByteBuffer> message) {
	// Query the SessionManager for the Session.
	std::shared_ptr<Session> session = session_manager_.GetSession(remote_endpoint);

	// If the Session doesnt exist, check for a Session Requesst.
	if(session == nullptr) {
		sessionless_messages_.push_back(make_shared<IncomingSessionlessPacket>(remote_endpoint, message));
	} else {
		incoming_messages_.push_back(make_shared<IncomingPacket>(session, message));
	}
}

std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> Service::event_dispatcher() {
    return event_dispatcher_;
}

void Service::event_dispatcher(std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher) {
    event_dispatcher_ = event_dispatcher;
}

SessionManager& Service::session_manager() {
    return session_manager_;
}

std::shared_ptr<Socket> Service::socket() {
    return socket_;
}
