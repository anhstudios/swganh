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
#include <anh/network/cluster/cluster_service.h>
#include <anh/event_dispatcher/basic_event.h>
#include <packets/RemoteMessage.h>

using namespace anh::event_dispatcher;
using namespace packets;

namespace anh {
namespace network {
namespace cluster {

PacketEventFilter::PacketEventFilter(ClusterService* service)
    : tbb::filter(serial_in_order)
    , service_(service)
{
}
PacketEventFilter::~PacketEventFilter() {}

void* PacketEventFilter::operator()(void* item)
{
    // grab ByteBuffer*
    anh::ByteBuffer *buffer = static_cast<ByteBuffer*>(item);
    if (buffer != nullptr)
    {
        // Fill out RemoteMessage
        RemoteMessage remote_message;
        remote_message.data = *buffer;
        remote_message.source_id = buffer->read<anh::HashString>();
        
        auto data_event = make_shared_event("RemoteMessage", remote_message);
        // trigger it here.
        // Note this gets handled by another dispatcher, who triggers it again after figuring out where it needs to go.
        service_->event_dispatcher_->triggerAsync(data_event);
    }
    return NULL;
}

} // cluster
} // network
} // anh