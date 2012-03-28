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

#include "swganh/messages/login_client_token.h"
#include "swganh/login/login_client.h"
#include "swganh/login/account.h"

using namespace swganh::login;
using namespace swganh::messages;
using namespace std;

using anh::ByteBuffer;

LoginClientToken swganh::messages::BuildLoginClientToken(shared_ptr<LoginClient> login_client, const std::string& session_key) {
	LoginClientToken message;
	
	ByteBuffer session_buffer;
	session_buffer.write(session_key);
	
	message.session_key = session_buffer;
	message.station_id = login_client->GetAccount()->account_id();
	message.station_username = login_client->GetUsername();
	
	return message;
}