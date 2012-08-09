// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_LOGIN_AUTHENTICATION_MANAGER_INTERFACE_H_
#define SWGANH_LOGIN_AUTHENTICATION_MANAGER_INTERFACE_H_

#include <memory>

#include "swganh/login/encoders/encoder_interface.h"

namespace swganh {
namespace login {

class Account;
class LoginClientInterface;

class AuthenticationManagerInterface {
public:

    virtual std::shared_ptr<encoders::EncoderInterface> encoder() = 0;

    virtual bool Authenticate(
        std::shared_ptr<swganh::login::LoginClientInterface> client, 
        std::shared_ptr<swganh::login::Account> account) = 0;
};

}}  // namespace swganh::login

#endif  // SWGANH_LOGIN_AUTHENTICATION_MANAGER_H_
