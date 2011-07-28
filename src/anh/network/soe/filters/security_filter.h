/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef ANH_NETWORK_SECURITY_FILTER_H_
#define ANH_NETWORK_SECURITY_FILTER_H_

#include <cstdint>
#include <memory>

#include <tbb/concurrent_queue.h>
#include <tbb/pipeline.h>

namespace anh {
namespace network {
namespace soe {
class Packet;
}}}  // namespace anh::network::soe

namespace anh {
namespace network {
namespace soe {
namespace filters {

/**
 * Performs basic checks for malformed packets.
 */
class SecurityFilter {
public:
    /**
     * @param max_receive_size Maximum allowed size of incoming messages.
     */
    SecurityFilter(uint32_t max_receive_size);

    /**
     * Processes messages waiting in the incoming queue.
     *
     * @param packet An IncomingPacket token to process.
     * @return An IncomingPacket token to process in the pipeline.
     */
    std::shared_ptr<Packet> operator()(std::shared_ptr<Packet> packet) const;

private:
    // Disable default construction.
    SecurityFilter();

    uint32_t max_receive_size_;
};

}}}} // namespace anh::network::soe::filters

#endif  // ANH_NETWORK_SECURITY_FILTER_H_
