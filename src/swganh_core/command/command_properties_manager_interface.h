// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <boost/optional.hpp>
#include "command_properties.h"

namespace swganh {
namespace command {
    
    /**
     * Defines an interface for retrieving command properties from an implementation
     * specific resource.
     */
    class CommandPropertiesManagerInterface
    {
    public:
        virtual ~CommandPropertiesManagerInterface() {}
        
        /**
         * Load a map of command properties from an implementation
         * specific resource.
         *
         * @return A map of command properties
         */
        virtual CommandPropertiesMap LoadCommandPropertiesMap() = 0;
        
        /**
         * Finds and returns the properties for a given command type.
         *
         * @param command A command name/crc to find.
         * @return An optional value containing a reference to the properties requested.
         */
        virtual boost::optional<const swganh::command::CommandProperties&> FindPropertiesForCommand(swganh::HashString command) = 0;
};
    
}}  // namespace swganh::command
