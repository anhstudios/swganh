// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <vector>

#include "swganh/command/command_validator_interface.h"

namespace swganh {
namespace command {
    class BaseSwgCommand;
}}

namespace swganh {
namespace command {

    class CommandValidator : public swganh::command::CommandValidatorInterface
    {
    public:
        virtual ~CommandValidator();

        virtual void AddCommandEnqueueFilter(swganh::command::CommandFilter&& filter);
        
        virtual void AddCommandProcessFilter(swganh::command::CommandFilter&& filter);

        virtual std::tuple<bool, uint32_t, uint32_t> ValidateForEnqueue(swganh::command::CommandInterface* command);
        
        virtual std::tuple<bool, uint32_t, uint32_t> ValidateForProcessing(swganh::command::CommandInterface* command);
        
    private:
        std::tuple<bool, uint32_t, uint32_t> ValidateCommand(swganh::command::CommandInterface* command, const std::vector<swganh::command::CommandFilter>& filters);

        std::vector<swganh::command::CommandFilter> enqueue_filters_;
        std::vector<swganh::command::CommandFilter> process_filters_;
    };

}}
