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

#include <anh/network/soe/service.h>
#include <anh/network/soe/socket.h>

namespace anh {
namespace network {
namespace soe {

Service::Service(void)
{
	//incoming_message_pipeline_.add_filter(crc_filter_);
	//incoming_message_pipeline_.add_filter(session_validation_filter_);
	//incoming_message_pipeline_.add_filter(decompression_filter_);
	//incoming_message_pipeline_.add_filter(decryption_filter_);

	//outgoing_message_pipeline_.add_filter(encryption_filter);
	//outgoing_message_pipeline_.add_filter(compression_filter);
}

Service::~Service(void)
{
}

void Service::Start(uint16_t port)
{
	// Create our socket.
	socket_ = std::make_shared<Socket>(io_service_, port, std::bind(&Service::OnSocketRecv_, this, std::placeholders::_1, std::placeholders::_2));
}

void Service::Update(void)
{
	// Run attached services.
	io_service_.run();
}

void Service::Shutdown(void)
{
	// Delete our socket.
	socket_.reset();
}

#include <assert.h>

void Service::OnSocketRecv_(boost::asio::ip::udp::endpoint& remote_endpoint, anh::ByteBuffer& message)
{
	assert(true && "You recieved a packet!");
}

} // namespace soe
} // namespace network
} // namespace anh