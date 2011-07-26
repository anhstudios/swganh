#include "login/messages/login_client_token.h"

#include "login/login_client.h"
#include "login/account.h"

using namespace login;
using namespace messages;
using namespace std;

using anh::ByteBuffer;

LoginClientToken login::messages::BuildLoginClientToken(shared_ptr<LoginClient> login_client, const std::string& session_key) {
    LoginClientToken message;

    ByteBuffer session_buffer;
    session_buffer.write(session_key);

    message.station_id = login_client->account->account_id();
    message.session_key = session_buffer;
    message.station_username = login_client->username;

    return message;
}
