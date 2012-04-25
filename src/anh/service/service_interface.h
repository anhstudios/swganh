// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef ANH_SERVICE_SERVICE_INTERFACE_H_
#define ANH_SERVICE_SERVICE_INTERFACE_H_

#include <boost/program_options/options_description.hpp>
#include "anh/service/service_description.h"

namespace anh {
namespace service {

class ServiceInterface {
public:
    virtual ~ServiceInterface() {}

    virtual ServiceDescription GetServiceDescription() = 0;

    /*
    * @brief Starts up the service, sets running_ to true
    */
    virtual void Start() = 0;
    /*
    * @brief Stops the service, sets running_ to false
    */
    virtual void Stop() = 0;

};

}}  // namespace anh::service


#endif  // ANH_SERVICE_SERVICE_INTERFACE_H_