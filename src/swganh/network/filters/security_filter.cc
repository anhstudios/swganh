// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "security_filter.h"

#include "swganh/byte_buffer.h"
#include "swganh/network/session.h"

using namespace swganh::network;
using swganh::ByteBuffer;

SecurityFilter::SecurityFilter(uint32_t max_receive_size)
    : max_receive_size_(max_receive_size)
{}

void SecurityFilter::operator()(Session* session, ByteBuffer* message) const
{
    uint32_t message_size = message->size();

    if (message_size > max_receive_size_)
    {
        /// @TODO: Track the number of errors for a session and set a threshhold where
        // their connection is dropped after a certain number of bad packets.
        
        throw std::runtime_error("Malformed message received: message larger than allowed size");
    }

    
    if (message_size < sizeof(uint16_t)) // Size of the message header
    {
        /// @TODO: Track the number of errors for a session and set a threshhold where
        // their connection is dropped after a certain number of bad packets.

        throw std::runtime_error("Malformed message received: message smaller than allowed size");
    }
}
