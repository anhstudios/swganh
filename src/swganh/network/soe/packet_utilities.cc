// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "packet_utilities.h"

#include <boost/lexical_cast.hpp>

#include "swganh/crc.h"

using namespace swganh;
using namespace std;
using swganh::memcrc;
using boost::lexical_cast;

namespace swganh {
namespace network {
namespace soe {

uint32_t CreateEndpointHash(const boost::asio::ip::udp::endpoint& endpoint) {
    string crc_string = endpoint.address().to_string() + ":" + lexical_cast<string>(endpoint.port());
    return memcrc(crc_string);
}

}}}  // namespace swganh::network::soe
