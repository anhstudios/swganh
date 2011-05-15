/*7262626262676
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

#include <anh/network/soe/session.h>
#include <anh/network/soe/session_manager.h>
#include <anh/network/soe/socket.h>
#include <anh/network/soe/protocol_opcodes.h>
#include <anh/utilities.h>
#include <anh/event_dispatcher/event_interface.h>

namespace anh {
namespace network {
namespace soe {

Session::Session(boost::asio::ip::udp::endpoint& remote_endpoint, std::shared_ptr<Socket> socket)
	: std::enable_shared_from_this<Session>()
	, remote_endpoint_(remote_endpoint)
	, connected_(false)
	, socket_(socket)
{
}

Session::~Session(void)
{
	if(connected_)
		Disconnect();
}

void Session::Update(void)
{
}

void Session::SendMessage(std::shared_ptr<anh::event_dispatcher::EventInterface> message)
{
	auto buffer = std::make_shared<ByteBuffer>();
	message->serialize(*buffer);
	outgoing_messages_.push(buffer);
}

void Session::SendMessage(std::shared_ptr<anh::ByteBuffer> message)
{
	outgoing_messages_.push(message);
}

void Session::SendMessage(char* buffer, uint16_t len)
{
	outgoing_messages_.push(std::make_shared<ByteBuffer>((const unsigned char*)buffer, len));
}

void Session::Disconnect(void)
{
	connected_ = false;
}

} // namespace anh
} // namespace network
} // namespace soe