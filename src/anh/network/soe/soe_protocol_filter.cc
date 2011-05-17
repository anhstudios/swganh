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

#include <anh/network/soe/soe_protocol_filter.h>
#include <anh/network/soe/service.h>
#include <anh/utilities.h>

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
	// A SESSION_REQUEST should never happen here.
	case SESSION_REQUEST:
		{
			break;
		}

	case MULTI_PACKET:
		{
			packet->session()->handleMultiPacket_(MultiPacket(*packet->message()));
			break;
		}

	case DISCONNECT:
		{
			packet->session()->handleDisconnect_(Disconnect(*packet->message()));
			break;
		}

	case PING:
		{
			packet->session()->handlePing_(Ping(*packet->message()));
			break;
		}

	case NET_STATS_CLIENT:
		{
			packet->session()->handleNetStatsClient_(NetStatsClient(*packet->message()));
			break;
		}

	case CHILD_DATA_A:
		{
			ChildDataA* packet_ = new ChildDataA(*packet->message());
			packet->session()->handleChildDataA_(*packet_);
			delete packet;
			return packet_;
			break;
		}

	case DATA_FRAG_A:
		{
			packet->session()->handleDataFragA_(DataFragA(*packet->message()));
			break;
		}

	case ACK_A:
		{
			packet->session()->handleAckA_(AckA(*packet->message()));
			break;
		}

	case FATAL_ERROR:
		{
			break;
		}

	default:
		LOG(WARNING) << "Unhandled SOE Opcode" << packet->message()->peek<uint16_t>(true);
	}

	delete packet;
	return NULL;
}

} // namespace soe
} // namespace network
} // namespace anh