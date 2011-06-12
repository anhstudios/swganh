/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2010 The SWG:ANH Team
---------------------------------------------------------------------------------------
Use of this source code is governed by the GPL v3 license that can be found
in the COPYING file or at http://www.gnu.org/licenses/gpl-3.0.html

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
---------------------------------------------------------------------------------------
*/

#ifndef ANH_NETWORK_SOE_RECV_PACKET_FILTER_H_
#define ANH_NETWORK_SOE_RECV_PACKET_FILTER_H_

#include <tbb/pipeline.h>

namespace anh {
namespace network {
namespace soe {

// FORWARD DELCARATIONS
class Service;

/**
 * @brief Pops an IncomingPacket off a Services incoming_message queue.
 *
 * This serial_in_order filter ensures the pipeline doesnt pop messages faster then its processing
 * them from the Service incoming_message queue.
 */
class RecvPacketFilter : public tbb::filter
{
public:
	/**
	 * @brief Constructor
	 *
	 * @param service The service this filter is associated with.
	 */
	RecvPacketFilter(Service* service);

	/**
	 * @brief Destructor
	 */
	~RecvPacketFilter(void);

	void* operator()(void* item);

private:
	Service*	service_;
};

} // namespace soe
} // namespace network
} // namespace anh

#endif