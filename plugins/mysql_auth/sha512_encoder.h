// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef MYSQL_AUTH_ENCODERS_SHA512_ENCODER_H_
#define MYSQL_AUTH_ENCODERS_SHA512_ENCODER_H_

#include "swganh/login/encoders/encoder_interface.h"
#include <memory>

namespace anh { namespace database { class DatabaseManagerInterface; 
}}  // anh::database

namespace plugins {
namespace mysql_auth {

class Sha512Encoder : public swganh::login::encoders::EncoderInterface {
public:
    explicit Sha512Encoder(anh::database::DatabaseManagerInterface* db_manager);
    ~Sha512Encoder();

    std::string EncodePassword(std::string raw, std::string salt);
    bool IsPasswordValid(std::string encoded, std::string raw, std::string salt);
private:
    anh::database::DatabaseManagerInterface* db_manager_;
};

}}  // namespace plugins::mysql_auth

#endif  // MYSQL_AUTH_ENCODERS_SHA512_ENCODER_H_
