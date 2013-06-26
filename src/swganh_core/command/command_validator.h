// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <vector>

#include "swganh_core/command/command_validator_interface.h"

namespace swganh {
namespace command {
    class BaseSwgCommand;
}}

namespace swganh {
namespace command {

	/**
	* Validates commands based on a series of filters
	*/
    class CommandValidator : public swganh::command::CommandValidatorInterface
    {
    public:

		/**
		* Adds a new pre-enqueue filter
		* @param filter the filter to add
		*/
        virtual void AddCommandEnqueueFilter(swganh::command::CommandFilter&& filter);
        
		/**
		* Adds a new process filter
		* @param filter the filter to add
		*/
        virtual void AddCommandProcessFilter(swganh::command::CommandFilter&& filter);

		/**
		* Runs the command through a series of filters in prep for enqueue
		* @param command the command to validate
		* @return a tuple representing success or failure.
		*/
        virtual std::tuple<bool, uint32_t, uint32_t> ValidateForEnqueue(swganh::command::CommandInterface* command);
        
		/**
		* Runs the command through a series of filters in prep for processing
		* @param command the command to validate
		* @return a tuple representing success or failure.
		*/
        virtual std::tuple<bool, uint32_t, uint32_t> ValidateForProcessing(swganh::command::CommandInterface* command);
        
    private:
        std::tuple<bool, uint32_t, uint32_t> ValidateCommand(swganh::command::CommandInterface* command, const std::vector<swganh::command::CommandFilter>& filters);

        std::vector<swganh::command::CommandFilter> enqueue_filters_;
        std::vector<swganh::command::CommandFilter> process_filters_;
    };

}}
