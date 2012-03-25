/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

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
