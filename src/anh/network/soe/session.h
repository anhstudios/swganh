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

#ifndef ANH_NETWORK_SOE_SESSION_H_
#define ANH_NETWORK_SOE_SESSION_H_

#include <boost/asio.hpp>

#include <anh/byte_buffer.h>
#include <anh/event_dispatcher/event_interface.h>

namespace anh {
namespace network {
namespace soe {

/**
 * @brief An estabilished connection between a SOE Client and a SOE Service.
 */
class Session
{
public:
	Session(boost::asio::ip::udp::endpoint& remote_endpoint);
	~Session(void);

	void SendMessage(std::shared_ptr<anh::event_dispatcher::EventInterface> message);
	void SendMessage(std::shared_ptr<anh::ByteBuffer> message);
	void SendMessage(char* buffer, uint16_t len);
	
	void SendProtocolMessage(std::shared_ptr<anh::ByteBuffer> message);
	void SendProtocolMessage(char* buffer, uint16_t len);

	void Disconnect(void);
private:
	boost::asio::ip::udp::endpoint		remote_endpoint_;
};

} // namespace soe
} // namespace network
} // namespace anh

#endif