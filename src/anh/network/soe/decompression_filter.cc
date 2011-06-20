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

#include "anh/network/soe/decompression_filter.h"

#include <array>
#include <zlib.h>

#include "anh/byte_buffer.h"
#include "anh/network/soe/incoming_packet.h"

using namespace anh;
using namespace network::soe;
using namespace std;

shared_ptr<IncomingPacket> DecompressionFilter::operator()(shared_ptr<IncomingPacket> packet) const {
	if((uint8_t)*(packet->message()->data() + (packet->message()->size() - 3)) == 1) // Check the 3rd to last byte of the packet for a compression flag.
	{
		Decompress_(packet->message());
	}

	return packet;
}

void DecompressionFilter::Decompress_(shared_ptr<ByteBuffer> buffer) const
{
	z_stream zstream_;
	array<unsigned char, 496> decompression_buffer_;

	zstream_.zalloc = Z_NULL;
	zstream_.zfree = Z_NULL;
	zstream_.opaque = Z_NULL;
	zstream_.avail_in= Z_NULL;
	zstream_.next_in = Z_NULL;
	inflateInit(&zstream_);

	zstream_.next_in = (Bytef*)buffer->data() + 2; // Start of the packet + 2 to pass opcode.
	zstream_.avail_in = buffer->size() - 5; // Do not process the SOE Opcode or the Footer.
	zstream_.next_out = (Bytef*)&decompression_buffer_[0] + 2; // Write passed the opcode.
	zstream_.avail_out = 496 - 5; // Don't write over the opcode.

	memcpy(&decompression_buffer_[0], buffer->data(), 2); // copy opcode.
	inflate(&zstream_, Z_FINISH); // Decompress Data
	inflateEnd(&zstream_);

	memcpy(&decompression_buffer_[0]+(zstream_.total_out - 3), buffer->data()+(buffer->size()-3), 3); // Copy Footer.
	buffer->swap(anh::ByteBuffer(&decompression_buffer_[0], zstream_.total_out + 5));
}
