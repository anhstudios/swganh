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

#include <anh/network/cluster/packet_event_filter.h>
#include <anh/network/cluster/service.h>
#include <packets/DataChannelMessage.h>

namespace anh {
namespace network {
namespace cluster {

PacketEventFilter::PacketEventFilter(Service* service)
    : tbb::filter(serial_in_order)
    , service_(service)
{
}
PacketEventFilter::~PacketEventFilter() {}

void* PacketEventFilter::operator()(void* item)
{
    // convert to TCPMessage*
    TCPMessage* message = static_cast<TCPMessage*>(item);
    if (message != nullptr)
    {
        // convert bytebuffer to our DataChannel message
        anh::ByteBuffer buffer = *message->message();
        packets::DataChannelMessage data_message(buffer);
        auto data_event = anh::event_dispatcher::make_shared_event("DataChannelMessage", data_message);
        // trigger it here.
        service_->event_dispatcher_->trigger(data_event);
    }
    return NULL;
}

} // cluster
} // network
} // anh