// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh_core/login/authentication_manager_interface.h"

namespace swganh {
namespace login {

/**
* Provides authentication via an encoder
*/
class AuthenticationManager : public swganh::login::AuthenticationManagerInterface {
public:
	/**
	* Creates a new instance
	*/
    explicit AuthenticationManager(std::shared_ptr<swganh::login::encoders::EncoderInterface> encoder);

	/**
	* @return the encoder used
	*/
    std::shared_ptr<swganh::login::encoders::EncoderInterface> encoder();

	/**
	* @return true if authenticated, false otherwise
	*/
    bool Authenticate(
        std::shared_ptr<swganh::login::LoginClientInterface> client, 
        std::shared_ptr<swganh::login::Account> account);

private:
    std::shared_ptr<swganh::login::encoders::EncoderInterface> encoder_;
};

}}  // namespace swganh::login

