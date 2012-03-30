/*
 This file is part of MMOServer. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2010 The SWG:ANH Team

 MMOServer is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 MMOServer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with MMOServer.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ANH_NETWORK_SOE_PACKET_UTILITIES_H_
#define ANH_NETWORK_SOE_PACKET_UTILITIES_H_

#include <cstdint>
#include <list>
#include <memory>

#include <boost/asio/ip/udp.hpp>

#include "anh/byte_buffer.h"

namespace anh {
namespace network {
namespace soe {

/**
 * Builds a data channel message header with the provided sequence.
 *
 * @param sequence The sequence of the data channel message header being created.
 * @return A byte buffer containing the fully constructed data channel message header.
 */
anh::ByteBuffer BuildDataChannelHeader(uint16_t sequence);

/**
 * Builds a fragmented data channel message header with the provided sequence.
 *
 * @param sequence The sequence of the data channel message header being created.
 * @return A byte buffer containing the fully constructed fragmented data channel message header.
 */
anh::ByteBuffer BuildFragmentedDataChannelHeader(uint16_t sequence);

/**
 * Packs a list of game messages into a single message body.
 *
 * @param data_list A list of game messages to pack.
 * @return A single data channel message containing 1 or more game messages.
 */
anh::ByteBuffer PackDataChannelMessages(std::list<anh::ByteBuffer> data_list);

/**
 * Splits a large data channel message into fragments.
 *
 * @param message The message to split.
 * @param max_size The maximum size of of each fragment.
 * @return A list containing the in-order message fragments.
 */
std::list<anh::ByteBuffer> SplitDataChannelMessage(anh::ByteBuffer message, uint32_t max_size);

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
        return anh::network::soe::CreateEndpointHash(endpoint); 
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

}}}  // namespace anh::network::soe

#endif // ANH_NETWORK_SOE_PACKET_UTILITIES_H_
