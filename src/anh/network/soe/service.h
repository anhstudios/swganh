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

#ifndef ANH_NETWORK_SOE_SERVICE_H_
#define ANH_NETWORK_SOE_SERVICE_H_

#include <anh/byte_buffer.h>

#include <map>
#include <stdint.h>

#include <boost/asio.hpp>

#include <tbb/pipeline.h>

namespace anh {
namespace network {
namespace soe {

// FORWARD DECLARATIONS
class Socket;

/**
 * @brief Represent an SOE Service, which takes incoming SOE Protocol packets on a specific port and filters them down to events and vise versa.
 */
class Service
{
public:
	Service(void);
	~Service(void);

	void Start(uint16_t port);
	void Update(void);
	void Shutdown(void);

private:
	void OnSocketRecv_(boost::asio::ip::udp::endpoint& remote_endpoint, anh::ByteBuffer& message);

	std::shared_ptr<Socket>		socket_;
	boost::asio::io_service		io_service_;

	tbb::pipeline	incoming_message_pipeline_;
	tbb::pipeline	outgoing_message_pipeline_;
};

} // namespace soe
} // namespace anh
} // namespace network

#endif // ANH_NETWORK_SOE_SERVER_H_