// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <boost/program_options/options_description.hpp>
#include "swganh/service/service_description.h"

namespace swganh {
namespace service {

class ServiceInterface {
public:
    virtual ~ServiceInterface() {}

    virtual ServiceDescription GetServiceDescription() = 0;

    /*
    * @brief Starts up the service, sets running_ to true
    */
    virtual void Startup() {}
    /*
    * @brief Stops the service, sets running_ to false
    */
    virtual void Shutdown() {}

};

}}  // namespace swganh::service