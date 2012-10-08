// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#pragma once

#include <cstdint>
#include <memory>

namespace swganh {

    class ByteBuffer;

namespace network {
namespace soe {

    class Session;

namespace filters {

    class DecryptionFilter
    {
    public:
        void operator()(Session* session, ByteBuffer* message) const;
    
    private:
    	int Decrypt_(char* buffer, uint32_t len, uint32_t seed) const;
    };

}}}} // namespace swganh::network::soe::filters
