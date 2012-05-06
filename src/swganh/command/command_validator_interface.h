// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_COMMAND_COMMAND_VALIDATOR_INTERFACE_H_
#define SWGANH_COMMAND_COMMAND_VALIDATOR_INTERFACE_H_

#include <cstdint>
#include <functional>
#include <tuple>

namespace swganh {
namespace command {
    
    class CommandInterface;

    typedef std::function<
        std::tuple<bool, uint32_t, uint32_t> (CommandInterface*)
    > CommandFilter;

    class CommandValidatorInterface
    {
    public:
        virtual ~CommandValidatorInterface() {}

        virtual void AddCommandEnqueueFilter(CommandFilter&& filter) = 0;
        
        virtual void AddCommandProcessFilter(CommandFilter&& filter) = 0;

        virtual std::tuple<bool, uint32_t, uint32_t> ValidateForEnqueue(CommandInterface* command) = 0;
        
        virtual std::tuple<bool, uint32_t, uint32_t> ValidateForProcessing(CommandInterface* command) = 0;
    };

}}

#endif  // SWGANH_COMMAND_COMMAND_VALIDATOR_INTERFACE_H_
