#ifndef PLUGINS_PHPBB_AUTH_PHPBB_ENCODER_H_
#define PLUGINS_PHPBB_AUTH_PHPBB_ENCODER_H_

#include <string>

#include "swganh/login/encoders/encoder_interface.h"

namespace plugins {
namespace phpbb_auth {

class PhpbbEncoder : public swganh::login::encoders::EncoderInterface 
{
public:
    PhpbbEncoder();

    std::string EncodePassword(std::string raw, std::string test);
    bool IsPasswordValid(std::string encoded, std::string raw, std::string salt);

private:
    std::string encode_(std::string input, int count);
    std::string md5_(std::string data);
    std::string itoa64_;
};

}}  // namespace plugins::phpbb_auth

#endif  // PLUGINS_PHPBB_AUTH_PHPBB_ENCODER_H_