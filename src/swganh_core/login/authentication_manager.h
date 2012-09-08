// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh/login/authentication_manager_interface.h"

namespace swganh {
namespace login {

class AuthenticationManager : public swganh::login::AuthenticationManagerInterface {
public:
    explicit AuthenticationManager(std::shared_ptr<swganh::login::encoders::EncoderInterface> encoder);

    std::shared_ptr<swganh::login::encoders::EncoderInterface> encoder();

    bool Authenticate(
        std::shared_ptr<swganh::login::LoginClientInterface> client, 
        std::shared_ptr<swganh::login::Account> account);

private:
    std::shared_ptr<swganh::login::encoders::EncoderInterface> encoder_;
};

}}  // namespace swganh::login

