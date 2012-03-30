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

#include "crc_out_filter.h"

#include "anh/byte_buffer.h"
#include "anh/crc.h"
#include "anh/network/soe/session.h"

using namespace anh;
using namespace network::soe;
using namespace filters;
using namespace std;

void CrcOutFilter::operator()(Session* session, ByteBuffer* message) 
{
    uint32_t packet_crc = memcrc(message->data(), message->size(), session->crc_seed());
    
    uint8_t crc_low = (uint8_t)packet_crc;
    uint8_t crc_high = (uint8_t)(packet_crc >> 8);
    
    message->write<uint8_t>(crc_high);
    message->write<uint8_t>(crc_low);
}
