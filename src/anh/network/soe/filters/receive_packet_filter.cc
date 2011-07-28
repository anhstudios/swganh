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

#include "anh/network/soe/filters/receive_packet_filter.h"

#include <glog/logging.h>

#include "anh/network/soe/packet.h"
#include "anh/network/soe/session.h"

using namespace anh::network::soe;
using namespace filters;
using namespace std;
using namespace tbb;

ReceivePacketFilter::ReceivePacketFilter(concurrent_queue<shared_ptr<Packet>>& incoming_queue, uint32_t max_receive_size)
    : incoming_queue_(incoming_queue)
    , max_receive_size_(max_receive_size) {}

shared_ptr<Packet> ReceivePacketFilter::operator() (flow_control& fc) const {    
    shared_ptr<Packet> packet = nullptr;

    do {
        // No more packets to process.
        if(incoming_queue_.empty()) {
            fc.stop();
            return nullptr;
        }

    } while(!incoming_queue_.try_pop(packet));
    
    if (packet->message()->size() > max_receive_size_) {
        DLOG(WARNING) << "Invalid message received";
        
        // @TODO: Track the number of errors for a session and set a threshhold where
        // their connection is dropped after a certain number of bad packets.

        packet = nullptr;
    }

    return packet;
}
