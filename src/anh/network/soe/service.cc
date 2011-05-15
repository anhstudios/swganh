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
#include <iostream>
#include <anh/byte_buffer.h>

#include <anh/network/soe/incoming_packet.h>
#include <anh/network/soe/incoming_sessionless_packet.h>
#include <anh/network/soe/service.h>
#include <anh/network/soe/session.h>
#include <anh/network/soe/socket.h>
#include <anh/network/soe/protocol_packets.h>

#ifdef ERROR
#undef ERROR
#endif

#include <glog/logging.h>

namespace anh {
namespace network {
namespace soe {

Service::Service(void)
	: session_request_filter_(this)
	, recv_packet_filter_(this)
	, crc_filter_(this, 0xDEADBABE)
	, decryption_filter_(this)
	, decompression_filter_(this)
{
	sessionless_incoming_pipeline_.add_filter(session_request_filter_);

	incoming_pipeline_.add_filter(recv_packet_filter_);
	incoming_pipeline_.add_filter(crc_filter_);
	incoming_pipeline_.add_filter(decryption_filter_);
	incoming_pipeline_.add_filter(decompression_filter_);
}

Service::~Service(void)
{	
}

void Service::Start(uint16_t port)
{
	// Create our socket.
	socket_ = std::make_shared<Socket>(io_service_, port, std::bind(&Service::OnSocketRecv_, this, std::placeholders::_1, std::placeholders::_2));
}

void Service::Update(void)
{
	incoming_pipeline_.run(1000);
	sessionless_incoming_pipeline_.run(1000);

	io_service_.poll();

	session_manager_.Update();
}

void Service::Shutdown(void)
{
	sessionless_incoming_pipeline_.clear();
	incoming_pipeline_.clear();
	socket_.reset();
}

void Service::OnSocketRecv_(boost::asio::ip::udp::endpoint& remote_endpoint, std::shared_ptr<anh::ByteBuffer> message)
{
	// Query the SessionManager for the Session.
	std::shared_ptr<Session> session = session_manager_.GetSession(remote_endpoint);

	// If the Session doesnt exist, check for a Session Requesst.
	if(session == nullptr)
	{
		sessionless_messages_.push_back(new IncomingSessionlessPacket(remote_endpoint, message));
	}
	else
	{
		incoming_messages_.push_back(new IncomingPacket(session, message));
	}
}

} // namespace soe
} // namespace network
} // namespace anh