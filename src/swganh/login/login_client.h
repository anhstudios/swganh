// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_LOGIN_LOGIN_CLIENT_H_
#define SWGANH_LOGIN_LOGIN_CLIENT_H_

#include <memory>
#include <string>

#include "anh/network/soe/session.h"

namespace swganh {
namespace login {

class Account;

class LoginClient : public anh::network::soe::Session {
public:    
    LoginClient(anh::network::soe::ServerInterface* server, boost::asio::io_service& io_service, boost::asio::ip::udp::endpoint remote_endpoint);
    
    std::string GetUsername() const;
    void SetUsername(std::string username);

    std::string GetPassword() const;
    void SetPassword(std::string password);

    std::string GetVersion() const;
    void SetVersion(std::string version);

    const std::shared_ptr<Account>& GetAccount() const;
    void SetAccount(const std::shared_ptr<Account>& account);

private:
    LoginClient();

    std::string username_;
    std::string password_;
    std::string version_;
    std::shared_ptr<Account> account_;
};

}}  // namespace swganh::login

#endif  // SWGANH_LOGIN_LOGIN_CLIENT_H_
