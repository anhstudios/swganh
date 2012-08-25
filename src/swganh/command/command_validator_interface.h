// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <functional>
#include <tuple>

namespace swganh {
namespace command {
    
    class CommandInterface;

    typedef std::function<
        std::tuple<bool, uint32_t, uint32_t> (CommandInterface*)
    > CommandFilter;

    /**
     * A validator used by the command service to validate commands requests.
     * 
     * This validation is applied globally to all commands when they are enqueued
     * and prior to processing.
     */
    class CommandValidatorInterface
    {
    public:
        virtual ~CommandValidatorInterface() {}

        /**
         * Adds a filter to use while validating a command prior to enqueuing.
         */
        virtual void AddCommandEnqueueFilter(CommandFilter&& filter) = 0;
        
        /**
         * Adds a filter to use while validating a command prior to processing.
         */
        virtual void AddCommandProcessFilter(CommandFilter&& filter) = 0;

        /**
         * Validates a command prior to enqueuing.
         *
         * @param command The command to validate.
         * @return A tuple containing a bool indicator if the command is valid and additional error
         *  information if it is not.
         */
        virtual std::tuple<bool, uint32_t, uint32_t> ValidateForEnqueue(CommandInterface* command) = 0;
        
        /**
         * Validates a command prior to processing.
         *
         * @param command The command to validate.
         * @return A tuple containing a bool indicator if the command is valid and additional error
         *  information if it is not.
         */
        virtual std::tuple<bool, uint32_t, uint32_t> ValidateForProcessing(CommandInterface* command) = 0;
    };

}}
