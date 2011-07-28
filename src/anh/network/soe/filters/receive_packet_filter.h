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

#ifndef ANH_NETWORK_SOE_RECEIVE_PACKET_FILTER_H_
#define ANH_NETWORK_SOE_RECEIVE_PACKET_FILTER_H_

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
 * Initial filter for use in a tbb parallel pipeline chain that processes incoming UDP
 * network packets.
 *
 * This serial_in_order filter is intended to be used as the first in a chain of filters for
 * processing incoming udp messages through a tbb parallel pipeline. It ensures the pipeline doesnt
 * pop messages faster then its processing them from the Service incoming_message queue.
 *
 * Each time a pipeline is invoked ReceivePacketFilter clears the contents of the queue it has been
 * given. The user is responsible for ensuring the queue is not being accessed otherwise during 
 * that time.
 */
class ReceivePacketFilter {
public:
    /**
     * Custom constructor that accepts a handle to a queue for incoming packets.
     *
     * @param incoming_queue A handle to the incoming queue used for processing.
     */
    explicit ReceivePacketFilter(tbb::concurrent_queue<std::shared_ptr<Packet>>& incoming_queue);

    /**
     * Processes messages waiting in the incoming queue.
     *
     * @param fc A flow control instance used to turn on and off the pipeline.
     * @return An IncomingPacket token to process in the pipeline.
     */
    std::shared_ptr<Packet> operator()(tbb::flow_control& fc) const;

private:
    // Disable default construction.
    ReceivePacketFilter();

    tbb::concurrent_queue<std::shared_ptr<Packet>>& incoming_queue_;
};

}}}} // namespace anh::network::soe::filters

#endif  // ANH_NETWORK_SOE_RECEIVE_PACKET_FILTER_H_
