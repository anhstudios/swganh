// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh/login/login_client_interface.h"

namespace swganh {
namespace login {

class LoginClient : public swganh::login::LoginClientInterface {
public:    
    LoginClient(swganh::network::soe::ServerInterface* server, boost::asio::io_service& io_service, boost::asio::ip::udp::endpoint remote_endpoint);
    
    std::string GetUsername() const;
    void SetUsername(std::string username);

    std::string GetPassword() const;
    void SetPassword(std::string password);

    std::string GetVersion() const;
    void SetVersion(std::string version);

    const std::shared_ptr<swganh::login::Account>& GetAccount() const;
    void SetAccount(const std::shared_ptr<swganh::login::Account>& account);

private:
    LoginClient();

    std::string username_;
    std::string password_;
    std::string version_;
    std::shared_ptr<swganh::login::Account> account_;
};

}}  // namespace swganh::login

