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

#include "anh/network/soe/filters/send_packet_filter.h"

#include <glog/logging.h>

#include "anh/byte_buffer.h"
#include "anh/network/soe/session.h"
#include "anh/network/soe/socket.h"
#include "anh/network/soe/packet.h"

using namespace anh::network::soe;
using namespace filters;
using namespace std;

SendPacketFilter::SendPacketFilter(Socket* socket)
    : socket_(socket) {}

void SendPacketFilter::operator()(const shared_ptr<Packet>& packet) const {   
    auto message = packet->message();

    try {
        socket_->Send(packet->session()->remote_endpoint(), *message);
    } catch(...) {
        DLOG(WARNING) << "Error sending remote message\n\n" << *message;
    }
}
