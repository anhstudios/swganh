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

#include <anh/network/soe/protocol_opcodes.h>
#include <anh/network/soe/soe_protocol_filter.h>
#include <anh/network/soe/service.h>
#include <anh/network/soe/session.h>
#include <anh/network/soe/session_manager.h>
#include <anh/network/soe/incoming_packet.h>

#ifdef ERROR
#undef ERROR
#endif

#include <glog/logging.h>

namespace anh {
namespace network {
namespace soe {

SoeProtocolFilter::SoeProtocolFilter(Service* service)
	: tbb::filter(true)
	, service_(service)
{
}

SoeProtocolFilter::~SoeProtocolFilter(void)
{
}

void* SoeProtocolFilter::operator()(void* item)
{
	IncomingPacket* packet = (IncomingPacket*)item;
	
	switch(packet->message()->peek<uint16_t>(true))
	{
	case SESSION_REQUEST:
		// This should never happen, because a session is already instantiated, just break.
		break;
	case MULTI_PACKET:
		HandleMultiPacket_(packet);
		break;
	case DISCONNECT:
		HandleDisconnect_(packet);
		break;
	case PING:
		HandlePing_(packet);
		break;
	case NET_STATS_CLIENT:
		HandleNetStatsClient_(packet);
		break;
	case CHILD_DATA_A:
		HandleChildDataA_(packet);
		break;
	case DATA_FRAG_A:
		HandleDataFragA_(packet);
		break;
	case OUT_OF_ORDER_A:
		HandleOutOfOrderA_(packet);
		break;
	case ACK_A:
		HandleAckA_(packet);
		break;
	case FATAL_ERROR:
		HandleFatalError_(packet);
		break;
	}

	return NULL;
}

void SoeProtocolFilter::HandleMultiPacket_(IncomingPacket* packet)
{
}

void SoeProtocolFilter::HandleDisconnect_(IncomingPacket* packet)
{
	LOG(WARNING) << "Session Disconnected. [" << packet->session()->remote_endpoint().address().to_string() << ":" << packet->session()->remote_endpoint().port() << "]";
	service_->session_manager_.RemoveSession(packet->session());
	packet->session()->Disconnect();
}

void SoeProtocolFilter::HandlePing_(IncomingPacket* packet)
{
}

void SoeProtocolFilter::HandleDataFragA_(IncomingPacket* packet)
{
}

void SoeProtocolFilter::HandleNetStatsClient_(IncomingPacket* packet)
{
}

void SoeProtocolFilter::HandleChildDataA_(IncomingPacket* packet)
{
}

void SoeProtocolFilter::HandleAckA_(IncomingPacket* packet)
{
}

void SoeProtocolFilter::HandleFatalError_(IncomingPacket* packet)
{
}

void SoeProtocolFilter::HandleOutOfOrderA_(IncomingPacket* packet)
{
}

} // namespace soe
} // namespace network
} // namespace anh