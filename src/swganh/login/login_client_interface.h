// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <memory>
#include <string>

#include "swganh/network/soe/session.h"

namespace swganh {
namespace login {

class Account;

class LoginClientInterface : public swganh::network::soe::Session {
public:    
    LoginClientInterface(swganh::network::soe::ServerInterface* server, boost::asio::io_service& io_service, boost::asio::ip::udp::endpoint remote_endpoint)
		:Session(server, io_service, remote_endpoint)
	{
	}
    
    virtual std::string GetUsername() const = 0;
    virtual void SetUsername(std::string username) = 0;

    virtual std::string GetPassword() const = 0;
    virtual void SetPassword(std::string password) = 0;

    virtual std::string GetVersion() const = 0;
    virtual void SetVersion(std::string version) = 0;

    virtual const std::shared_ptr<Account>& GetAccount() const = 0;
    virtual void SetAccount(const std::shared_ptr<Account>& account) = 0;
};

}}  // namespace swganh::login
