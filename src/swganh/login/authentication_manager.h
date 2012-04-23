// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_LOGIN_AUTHENTICATION_MANAGER_H_
#define SWGANH_LOGIN_AUTHENTICATION_MANAGER_H_

#include <memory>

#include "swganh/login/encoders/encoder_interface.h"

namespace swganh {
namespace login {

class Account;
class LoginClient;

class AuthenticationManager {
public:
    explicit AuthenticationManager(std::shared_ptr<encoders::EncoderInterface> encoder);

    std::shared_ptr<encoders::EncoderInterface> encoder();

    bool Authenticate(
        std::shared_ptr<swganh::login::LoginClient> client, 
        std::shared_ptr<swganh::login::Account> account);

private:
    std::shared_ptr<encoders::EncoderInterface> encoder_;
};

}}  // namespace swganh::login

#endif  // SWGANH_LOGIN_AUTHENTICATION_MANAGER_H_
