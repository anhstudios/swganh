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

#include "anh/network/soe/filters/security_filter.h"

#include <glog/logging.h>

#include "anh/network/soe/packet.h"
#include "anh/network/soe/session.h"

using namespace anh::network::soe;
using namespace filters;
using namespace std;

SecurityFilter::SecurityFilter(uint32_t max_receive_size)
    : max_receive_size_(max_receive_size) {}

shared_ptr<Packet> SecurityFilter::operator()(shared_ptr<Packet> packet) const {
    if (!packet) { return nullptr; }

    auto message_size = packet->message()->size();

    if (packet->message()->size() > max_receive_size_) {
        DLOG(WARNING) << "Malformed message received: message larger than allowed size";
        
        // @TODO: Track the number of errors for a session and set a threshhold where
        // their connection is dropped after a certain number of bad packets.

        packet = nullptr;
    }

    
    if (message_size <= sizeof(uint16_t)) { // Size of the message header
        DLOG(WARNING) << "Malformed message received: message smaller than allowed size";
        
        // @TODO: Track the number of errors for a session and set a threshhold where
        // their connection is dropped after a certain number of bad packets.

        packet = nullptr;
    }

    return packet;
}
