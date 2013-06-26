// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include <zlib.h>

#include "swganh/byte_buffer.h"

namespace swganh {
namespace network {

    class Session;

    /**
     * @brief Decompresses packet data that is flagged as compressed.
     */
    class DecompressionFilter {
    public:
        /**
         * @param max_receive_size Maximum allowed size of incoming messages.
         */
        explicit DecompressionFilter(uint32_t max_message_size);
    
        void operator()(Session* session, ByteBuffer* message);
    
    private:
        DecompressionFilter();
        
        uint32_t max_message_size_;
        
        z_stream zstream_;
        ByteBuffer zbuffer_;
    };

}} // namespace swganh::network
