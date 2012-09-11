// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh/login/encoders/encoder_interface.h"
#include <memory>

namespace swganh { namespace database { class DatabaseManagerInterface; 
}}  // swganh::database

namespace swganh {
namespace login {

class Sha512Encoder : public swganh::login::encoders::EncoderInterface {
public:
    explicit Sha512Encoder(swganh::database::DatabaseManagerInterface* db_manager);
    ~Sha512Encoder();

    std::string EncodePassword(std::string raw, std::string salt);
    bool IsPasswordValid(std::string encoded, std::string raw, std::string salt);
private:
    swganh::database::DatabaseManagerInterface* db_manager_;
};

}}  // namespace swganh::login

