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

#include <anh/network/soe/service.h>
#include <anh/byte_buffer.h>
#include <anh/network/soe/incoming_packet.h>
#include <anh/network/soe/incoming_sessionless_packet.h>
#include <anh/network/soe/session.h>
#include <anh/network/soe/socket.h>
#include <anh/network/soe/protocol_packets.h>

using namespace std;

namespace anh {
namespace network {
namespace soe {

#pragma warning(push)
#pragma warning(disable:4355)
Service::Service()
	: event_dispatcher_(nullptr)
    , crc_seed_(0xDEADBABE)
	, session_request_filter_(this)
	, recv_packet_filter_(this)
	, compression_filter_(this)
	, crc_in_filter_(this)
	, decryption_filter_(this)
	, decompression_filter_(this)
	, soe_protocol_filter_(this)
	, outgoing_start_filter_(this)
	, encryption_filter_(this)
	, crc_out_filter_(this)
	, send_packet_filter_(this)
{
    sessionless_filter_ = tbb::make_filter<void, void>(tbb::filter::serial_in_order, session_request_filter_);
    
    incoming_filter_ = 
        tbb::make_filter<void, shared_ptr<IncomingPacket>>(tbb::filter::serial_in_order, recv_packet_filter_)
        &
        tbb::make_filter<shared_ptr<IncomingPacket>, shared_ptr<IncomingPacket>>(tbb::filter::parallel, crc_in_filter_)
        &
        tbb::make_filter<shared_ptr<IncomingPacket>, shared_ptr<IncomingPacket>>(tbb::filter::parallel, decryption_filter_)
        &
        tbb::make_filter<shared_ptr<IncomingPacket>, shared_ptr<IncomingPacket>>(tbb::filter::parallel, decompression_filter_)
        &
        tbb::make_filter<shared_ptr<IncomingPacket>, void>(tbb::filter::serial_in_order, soe_protocol_filter_);
    
    
    outgoing_filter_ = 
        tbb::make_filter<void, shared_ptr<OutgoingPacket>>(tbb::filter::serial_in_order, outgoing_start_filter_)
        &
        tbb::make_filter<shared_ptr<OutgoingPacket>, shared_ptr<OutgoingPacket>>(tbb::filter::parallel, compression_filter_)
        &
        tbb::make_filter<shared_ptr<OutgoingPacket>, shared_ptr<OutgoingPacket>>(tbb::filter::parallel, encryption_filter_)
        &
        tbb::make_filter<shared_ptr<OutgoingPacket>, shared_ptr<OutgoingPacket>>(tbb::filter::parallel, crc_out_filter_)
        &
        tbb::make_filter<shared_ptr<OutgoingPacket>, void>(tbb::filter::serial_in_order, send_packet_filter_);
}
#pragma warning(pop)

Service::~Service(void)
{	
}

void Service::Start(uint16_t port)
{
	socket_ = std::make_shared<Socket>(io_service_, port, std::bind(&Service::OnSocketRecv_, this, std::placeholders::_1, std::placeholders::_2));
}

void Service::Update(void)
{
	io_service_.poll();

    tbb::parallel_pipeline(1000, sessionless_filter_);
    tbb::parallel_pipeline(1000, incoming_filter_);
    tbb::parallel_pipeline(1000, outgoing_filter_);

	session_manager_.Update();
}

void Service::Shutdown(void)
{
	sessionless_incoming_pipeline_.clear();
	incoming_pipeline_.clear();
	outgoing_pipeline_.clear();

	socket_.reset();
}

void Service::OnSocketRecv_(boost::asio::ip::udp::endpoint remote_endpoint, std::shared_ptr<anh::ByteBuffer> message)
{
	// Query the SessionManager for the Session.
	std::shared_ptr<Session> session = session_manager_.GetSession(remote_endpoint);

	// If the Session doesnt exist, check for a Session Requesst.
	if(session == nullptr)
	{
		sessionless_messages_.push_back(make_shared<IncomingSessionlessPacket>(remote_endpoint, message));
	}
	else
	{
		incoming_messages_.push_back(make_shared<IncomingPacket>(session, message));
	}
}

std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> Service::event_dispatcher() {
    return event_dispatcher_;
}

void Service::event_dispatcher(std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher) {
    event_dispatcher_ = event_dispatcher;
}

} // namespace soe
} // namespace network
} // namespace anh