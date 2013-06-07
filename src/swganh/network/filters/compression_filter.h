// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#pragma once

#include <zlib.h>
#include "swganh/byte_buffer.h"

namespace swganh {
namespace network {

class Session;

class CompressionFilter {
public:    
    void operator()(Session* session, ByteBuffer* message);

private:
    z_stream zstream_;
    ByteBuffer zbuffer_;
};

}} // namespace swganh::network
