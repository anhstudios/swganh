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

#include "anh/network/soe/compression_filter.h"

#include <array>

#include <zlib.h>

#include "anh/network/soe/outgoing_packet.h"

using namespace anh;
using namespace network::soe;
using namespace std;

shared_ptr<OutgoingPacket> CompressionFilter::operator()(shared_ptr<OutgoingPacket> packet) const {
	// See if compression is flagged for this message.
	if(packet->message()->peekAt<uint8_t>(packet->message()->size() - 3) == 0x01)
		Compress_(*packet->message());
	
	return packet;
}

void CompressionFilter::Compress_(ByteBuffer& buffer) const {
	z_stream zstream_;
	array<unsigned char, 496> compression_buffer_;

	zstream_.zalloc = Z_NULL;
	zstream_.zfree = Z_NULL;
	zstream_.opaque = Z_NULL;
	zstream_.avail_in = Z_NULL;
	zstream_.next_in = Z_NULL;
	deflateInit(&zstream_, Z_DEFAULT_COMPRESSION);

	zstream_.next_in = (Bytef*)buffer.data() + 2;
	zstream_.avail_in = buffer.size() - 5;
	zstream_.next_out = (Bytef*)&compression_buffer_[0]+2;
	zstream_.avail_out = 496 - 5;

	memcpy(&compression_buffer_[0], buffer.data(), 2); // copy opcode.
	deflate(&zstream_, Z_FINISH);
	deflateEnd(&zstream_);

	memcpy(&compression_buffer_[0]+(zstream_.total_out - 3), buffer.data()-(buffer.size() - 3), 3); // copy footer
	buffer.swap(ByteBuffer(&compression_buffer_[0], zstream_.total_out));
}
