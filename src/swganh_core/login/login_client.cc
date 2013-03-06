// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "login_client.h"
#include "swganh/observer/observer_interface.h"

using namespace swganh::network::soe;
using namespace std;
using namespace swganh::login;
using namespace swganh::login;

LoginClient::LoginClient(
    ServerInterface* server, boost::asio::io_service& cpu_pool, boost::asio::ip::udp::endpoint remote_endpoint)
    : LoginClientInterface(server, cpu_pool, remote_endpoint)
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
