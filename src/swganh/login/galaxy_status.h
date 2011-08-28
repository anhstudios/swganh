
#ifndef SWGANH_LOGIN_GALAXY_STATUS_H_
#define SWGANH_LOGIN_GALAXY_STATUS_H_

#include <cstdint>
#include <string>

namespace swganh {
namespace login {

struct GalaxyStatus {    
    uint32_t galaxy_id;
    std::string name;
    std::string address;
    uint16_t connection_port;
    uint16_t ping_port;
    uint32_t server_population;
    uint32_t max_population;
    uint32_t max_characters;
    uint32_t distance;
    uint32_t status;
};

}}  // namespace swganh::login

#endif  // SWGANH_LOGIN_GALAXY_STATUS_H_
