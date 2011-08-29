#include "swganh/login/messages/login_client_token.h"

#include "swganh/login/login_client.h"
#include "swganh/login/account.h"

using namespace swganh::login;
using namespace messages;
using namespace std;

using anh::ByteBuffer;

LoginClientToken swganh::login::messages::BuildLoginClientToken(shared_ptr<swganh::login::LoginClient> login_client, const std::string& session_key) {
    LoginClientToken message;

    ByteBuffer session_buffer;
    session_buffer.write(session_key);

    message.station_id = login_client->account->account_id();
    message.session_key = session_buffer;
    message.station_username = login_client->username;

    return message;
}
