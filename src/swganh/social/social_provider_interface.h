// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_SOCIAL_PROVIDER_INTERFACE_H_
#define SWGANH_SOCIAL_PROVIDER_INTERFACE_H_

#include <cstdint>
#include <tuple>
#include <vector>


#include "swganh/character/character_data.h"
#include "pub14_core/messages/client_create_character.h"

namespace swganh {
namespace character {
    
class SocialProviderInterface {
public:
    virtual ~SocialProviderInterface() {}
    
};

}}  // namespace swganh::character

#endif  // SWGANH_SOCIAL_PROVIDER_INTERFACE_H_

