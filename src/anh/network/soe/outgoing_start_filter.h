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

#ifndef ANH_NETWORK_SOE_OUTGOING_START_FILTER_H_
#define ANH_NETWORK_SOE_OUTGOING_START_FILTER_H_

#include <list>
#include <memory>
#include <tbb/pipeline.h>

namespace anh {
namespace network {
namespace soe {

// FORWARD DECLARATIONS
class OutgoingPacket;
/**
 * @brief Retrieves the next outgoing message off outgoing message queue.
 */
class OutgoingStartFilter
{
public:
	OutgoingStartFilter(std::list<std::shared_ptr<OutgoingPacket>>& message_queue);

	std::shared_ptr<OutgoingPacket> operator()(tbb::flow_control& fc) const;

private:
	std::list<std::shared_ptr<OutgoingPacket>>& message_queue_;
};

}}} // namespace anh::network::soe

#endif // ANH_NETWORK_SOE_OUTGOING_START_FILTER_H_
