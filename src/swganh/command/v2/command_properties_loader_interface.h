// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_COMMAND_V2_COMMAND_PROPERTIES_LOADER_INTERFACE_H_
#define SWGANH_COMMAND_V2_COMMAND_PROPERTIES_LOADER_INTERFACE_H_

#include "command_properties.h"

namespace swganh {
namespace command {
namespace v2 {
    
    /**
     * Defines an interface for retrieving command properties from an
     * implementation specific resource.
     */
    class CommandPropertiesLoaderInterface
    {
    public:
        virtual ~CommandPropertiesLoaderInterface() {}
        
        /**
         * Load a map of command properties from an implementation
         * specific resource.
         *
         * @return A map of command properties
         */
        virtual CommandPropertiesMap LoadCommandPropertiesMap() = 0;
    };
    
}}}  // namespace swganh::command::v2

#endif  // SWGANH_COMMAND_V2_COMMAND_PROPERTIES_LOADER_INTERFACE_H_
