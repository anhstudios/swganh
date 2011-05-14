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

#include <anh/network/soe/session_request_filter.h>
#include <anh/byte_buffer.h>
#include <anh/network/soe/protocol_opcodes.h>
#include <anh/network/soe/session.h>
#include <anh/network/soe/session_manager.h>
#include <anh/network/soe/service.h>
#include <anh/network/soe/incoming_sessionless_packet.h>
#include <anh/utilities.h>

namespace anh {
namespace network {
namespace soe {

SessionRequestFilter::SessionRequestFilter(Service* service)
	: tbb::filter(false)
	, service_(service)
{
}

SessionRequestFilter::~SessionRequestFilter(void)
{
}

void* SessionRequestFilter::operator()(void* item)
{
	std::shared_ptr<IncomingSessionlessPacket> sessionless_message = service_->sessionless_messages_.back();
	if(sessionless_message->message()->read<uint16_t>(true) == SESSION_REQUEST)
	{
		// Create Session
		auto session = std::make_shared<Session>(sessionless_message->remote_endpoint(), service_->session_manager_);

		// Get packet values.
		sessionless_message->message()->read<uint32_t>(true); // crc length
		session->set_connection_id(sessionless_message->message()->read<uint32_t>(true)); // connection id
		session->set_recv_buffer_size(sessionless_message->message()->read<uint32_t>(true)); // udp buffer size

		// Send Session Response
		anh::ByteBuffer session_response;
		session_response.write<uint16_t>(anh::bigToHost<uint16_t>(SESSION_RESPONSE));
		session_response.write<uint32_t>(anh::bigToHost<uint32_t>(session->connection_id()));
		session_response.write<uint32_t>(0xDEADBABE); // crc seed
		session_response.write<uint8_t>(2); // crc len
		session_response.write<uint8_t>(2); // encryption type
		session_response.write<uint8_t>(4); // seed length
		session_response.write<uint32_t>(anh::bigToHost<uint32_t>(456)); // Server UDP Max Buffer
		service_->socket_->Send(session->remote_endpoint(), session_response);
	}

	service_->sessionless_messages_.pop();
	return 0;
}

} // namespace soe
} // namespace network
} // namespace anh