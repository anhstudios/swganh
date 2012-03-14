#ifndef PLUGINS_SMF_AUTH_SMF_ENCODER_H_
#define PLUGINS_SMF_AUTH_SMF_ENCODER_H_

#include <string>
#include <memory>

#include "swganh/login/encoders/encoder_interface.h"

namespace anh {
namespace database {
    class DatabaseManagerInterface;
}}  // namespace anh::database

namespace plugins {
namespace smf_auth {

class SmfEncoder : public swganh::login::encoders::EncoderInterface 
{
public:
    SmfEncoder(anh::database::DatabaseManagerInterface* database_manager);

    std::string EncodePassword(std::string raw, std::string test);
    bool IsPasswordValid(std::string encoded, std::string raw, std::string salt);
    
private:
    anh::database::DatabaseManagerInterface* database_manager_;
};

}}  // namespace plugins::smf_auth

#endif  // PLUGINS_SMF_AUTH_SMF_ENCODER_H_
