// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh_core/login/encoders/encoder_interface.h"
#include <memory>

namespace swganh { namespace database { class DatabaseManager; 
}}  // swganh::database

namespace swganh {
namespace login {

/**
* Provides Sha512 encoding for password checking via MYSQL
*/
class Sha512Encoder : public swganh::login::encoders::EncoderInterface {
public:

	/**
	* Creates a new instance
	*/
    explicit Sha512Encoder(swganh::database::DatabaseManager* db_manager);

	/**
	* Encodes a password with the given salt and returns the value
	*/
    std::string EncodePassword(std::string raw, std::string salt);

	/**
	* Compares the encoded string to the raw when encoded with the salt
	*/
    bool IsPasswordValid(std::string encoded, std::string raw, std::string salt);
private:
    swganh::database::DatabaseManager* db_manager_;
};

}}  // namespace swganh::login

