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

class EncryptionFilter {
public:
    void operator()(Session* session, ByteBuffer* message);

private:
	void Encrypt_(char* buffer, uint32_t len, uint32_t seed) const;
};

}}}} // namespace swganh::network::soe::filters
