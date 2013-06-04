// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>

#include <boost/asio/ip/udp.hpp>

namespace swganh {
namespace network {
namespace soe {

/**
 * Creates a uint32_t hash from an endpoint.
 *
 * @param endpoint The endpoint to hash.
 * @return A hash of the provided endpoint.
 */
uint32_t CreateEndpointHash(const boost::asio::ip::udp::endpoint& endpoint);

class EndpointHash {
  public:
    size_t operator ()(const boost::asio::ip::udp::endpoint& endpoint) const { 
        return swganh::network::soe::CreateEndpointHash(endpoint); 
    }
};

class EndpointEqual {
  public:
    bool operator ()(
        const boost::asio::ip::udp::endpoint& endpoint1, 
        const boost::asio::ip::udp::endpoint& endpoint2) const
    {
        return endpoint1 == endpoint2;
    }
};

}}}  // namespace swganh::network::soe
