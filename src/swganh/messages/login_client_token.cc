// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "login_client_token.h"

#include "swganh/login/login_client.h"
#include "swganh/login/account.h"

using namespace swganh::login;
using namespace swganh::messages;
using namespace std;

using anh::ByteBuffer;

LoginClientToken swganh::messages::BuildLoginClientToken(
    shared_ptr<LoginClient> login_client, 
    const string& session_key)
{
	LoginClientToken message;
	
	ByteBuffer session_buffer;
	session_buffer.write(session_key);
	
	message.session_key = session_buffer;
	message.station_id = login_client->GetAccount()->account_id();
	message.station_username = login_client->GetUsername();
	
	return message;
}
