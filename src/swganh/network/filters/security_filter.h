// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#pragma once

#include <cstdint>
#include <memory>

namespace swganh {

    class ByteBuffer;

namespace network {

    class Session;

    /**
     * Performs basic checks for malformed packets.
     */
    class SecurityFilter {
    public:
        /**
         * @param max_receive_size Maximum allowed size of incoming messages.
         */
        explicit SecurityFilter(uint32_t max_receive_size);
    
        void operator()(Session* session, ByteBuffer* message) const;
    
    private:
        // Disable default construction.
        SecurityFilter();
    
        uint32_t max_receive_size_;
    };

}} // namespace swganh::network
