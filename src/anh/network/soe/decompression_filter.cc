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

#include <anh/network/soe/decompression_filter.h>
#include <anh/network/soe/service.h>
#include <anh/network/soe/incoming_packet.h>

#include <zlib.h>

#ifdef ERROR
#undef ERROR
#endif

#include <glog/logging.h>

namespace anh {
namespace network {
namespace soe {

DecompressionFilter::DecompressionFilter(Service* service)
	: tbb::filter(parallel)
	, service_(service)
	, decompression_buffer_(new char[496])
{
}

DecompressionFilter::~DecompressionFilter(void)
{
}

void* DecompressionFilter::operator()(void* item)
{
	IncomingPacket* packet = (IncomingPacket*)item;
	if((uint8_t)*(packet->message()->data() + (packet->message()->size() - 3)) == 1) // Check the 3rd to last byte of the packet for a compression flag.
	{
		Decompress_(packet->message());
	}

	return packet;
}

void DecompressionFilter::Decompress_(std::shared_ptr<anh::ByteBuffer> buffer)
{
	zstream_.zalloc = Z_NULL;
	zstream_.zfree = Z_NULL;
	zstream_.opaque = Z_NULL;
	zstream_.avail_in= Z_NULL;
	zstream_.next_in = Z_NULL;
	inflateInit(&zstream_);

	zstream_.next_in = (Bytef*)buffer->data() + 2; // Start of the packet + 2 to pass opcode.
	zstream_.avail_in = buffer->size() - 5; // Do not process the SOE Opcode or the Footer.
	zstream_.next_out = (Bytef*)decompression_buffer_ + 2; // Write passed the opcode.
	zstream_.avail_out = 496 - 5; // Don't write over the opcode.

	memcpy(decompression_buffer_, buffer->data(), 2); // copy opcode.
	inflate(&zstream_, Z_FINISH); // Decompress Data
	inflateEnd(&zstream_);

	memcpy(decompression_buffer_+(zstream_.total_out - 3), buffer->data()+(buffer->size()-3), 3); // Copy Footer.
	buffer->swap(anh::ByteBuffer((const unsigned char*)decompression_buffer_, zstream_.total_out + 5));
}

} // namespace soe
} // namespace network
} // namespace anh