// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <string>

namespace swganh {
namespace login {
namespace encoders {

class EncoderInterface {
public:
    virtual ~EncoderInterface() {}

    virtual std::string EncodePassword(std::string raw, std::string salt) = 0;
    virtual bool IsPasswordValid(std::string encoded, std::string raw, std::string salt) = 0;
};

}}}  // namespace swganh::login::encoders
