// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef ANH_NETWORK_SOE_ENCRYPTION_FILTER_H_
#define ANH_NETWORK_SOE_ENCRYPTION_FILTER_H_

#include <cstdint>
#include <memory>

namespace anh {

    class ByteBuffer;

namespace network {
namespace soe {

    class Session;

namespace filters {

class EncryptionFilter {
public:
    void operator()(Session* session, ByteBuffer* message);

private:
	void Encrypt_(char* buffer, uint32_t len, uint32_t seed) const;
};

}}}} // namespace anh::network::soe::filters

#endif // ANH_NETWORK_SOE_DECOMPRESSION_FILTER_H_
