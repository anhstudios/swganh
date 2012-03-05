
#include "swganh/login/login_client.h"
#include "swganh/object/object_controller.h"

using namespace anh::network::soe;
using namespace std;
using namespace swganh::login;
using namespace swganh::object;

LoginClient::LoginClient(
    ServerInterface* server, boost::asio::io_service& io_service, boost::asio::ip::udp::endpoint remote_endpoint)
    : Session(server, io_service, remote_endpoint)
{}

string LoginClient::GetUsername() const
{
    return username_;
}

void LoginClient::SetUsername(string username)
{
    username_ = username;
}

string LoginClient::GetPassword() const
{
    return password_;
}

void LoginClient::SetPassword(string password)
{
    password_ = password;
}

string LoginClient::GetVersion() const
{
    return version_;
}

void LoginClient::SetVersion(string version)
{
    version_ = version;
}

const shared_ptr<Account>& LoginClient::GetAccount() const
{
    return account_;
}

void LoginClient::SetAccount(const shared_ptr<Account>& account)
{
    account_ = account;
}
