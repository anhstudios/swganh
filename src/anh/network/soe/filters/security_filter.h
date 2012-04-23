// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef ANH_NETWORK_SECURITY_FILTER_H_
#define ANH_NETWORK_SECURITY_FILTER_H_

#include <cstdint>
#include <memory>

namespace anh {

    class ByteBuffer;

namespace network {
namespace soe {

    class Session;

namespace filters {

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

}}}} // namespace anh::network::soe::filters

#endif  // ANH_NETWORK_SECURITY_FILTER_H_
