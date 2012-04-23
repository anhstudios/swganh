// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef ANH_NETWORK_SOE_COMPRESSION_FILTER_H_
#define ANH_NETWORK_SOE_COMPRESSION_FILTER_H_

namespace anh {

    class ByteBuffer;

namespace network {
namespace soe {

    class Session;

namespace filters {

class CompressionFilter {
public:    
    void operator()(Session* session, ByteBuffer* message);

private:
	void Compress_(ByteBuffer* message);
};

}}}} // namespace anh::network::soe::filters

#endif // ANH_NETWORK_SOE_DECOMPRESSION_FILTER_H_
