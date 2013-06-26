// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#pragma once

namespace swganh {

    class ByteBuffer;

namespace network {
    
    class Session;

    struct CrcInFilter 
    {
    public:
        void operator()(Session* session, ByteBuffer* message) const;
    };

}} // namespace swganh::network
