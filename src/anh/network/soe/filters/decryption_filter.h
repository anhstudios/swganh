// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef ANH_NETWORK_SOE_DECRYPTION_FILTER_H_
#define ANH_NETWORK_SOE_DECRYPTION_FILTER_H_

#include <cstdint>
#include <memory>

namespace anh {

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

}}}} // namespace anh::network::soe::filters

#endif // ANH_NETWORK_SOE_DECOMPRESSION_FILTER_H_
