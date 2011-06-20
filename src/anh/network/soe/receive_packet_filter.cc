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

#include "anh/network/soe/receive_packet_filter.h"
#include "anh/network/soe/incoming_packet.h"
#include "anh/network/soe/session.h"

using namespace anh::network::soe;
using namespace std;
using namespace tbb;

ReceivePacketFilter::ReceivePacketFilter(list<shared_ptr<IncomingPacket>>& incoming_queue)
    : incoming_queue_(incoming_queue) {}

shared_ptr<IncomingPacket> ReceivePacketFilter::operator() (flow_control& fc) const {
    shared_ptr<IncomingPacket> packet;

    do {
        // No more packets to process.
        if(incoming_queue_.empty()) {
            fc.stop();
            return nullptr;
        }

        packet = incoming_queue_.front();
        incoming_queue_.pop_front();

        // Loop until we have a packet with a valid connection.
    } while(packet->session()->connected() == false);

    return packet;
}
