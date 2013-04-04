// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <tuple>

#include <boost/optional.hpp>

#include "swganh/service/service_interface.h"

#include "command_factory_interface.h"
#include "command_validator_interface.h"
#include "command_properties_manager_interface.h"

namespace swganh {
namespace observer {
	class ObserverInterface;
}
namespace object {
    class Object;
	class Tangible;
	class Creature;
} // namespace object

namespace simulation {
    class SimulationServiceInterface;
}}  // namespace swganh::simulation;


namespace swganh {
namespace command {

    class CommandInterface;
    class CommandQueueInterface;
    class CommandQueueManagerInterface;

    /**
     * The command service is responsible for the handling of incoming command
     * requests for game objects.
     */
    class CommandServiceInterface: public swganh::service::BaseService
    {
    public:
        virtual ~CommandServiceInterface() {}
        
        /**
         * Adds a filter to use while validating a command prior to enqueuing.
         */
        virtual void AddCommandEnqueueFilter(CommandFilter&& filter) = 0;
        
        /**
         * Adds a filter to use while validating a command prior to processing.
         */
        virtual void AddCommandProcessFilter(CommandFilter&& filter) = 0;
        
        template<typename T>
        void AddCommandCreator(swganh::HashString command)
        {
            AddCommandCreator(command, [] () { return std::make_shared<T>(); });
        }

        /**
         * Adds a creator for a given command type. Only the most recently added creator for
         * a type is used.
         *
         * @param command The name/crc of the command.
         * @param creator The creator associated with the specified command.
         */
        virtual void AddCommandCreator(swganh::HashString command, CommandCreator creator) = 0;
        
        /**
         * Removes the creator for a given command type if one is set.
         *
         * @param command The name/crc of the command.
         */
        virtual void RemoveCommandCreator(swganh::HashString command) = 0;

        virtual std::shared_ptr<CommandInterface> CreateCommand(swganh::HashString command) = 0;
                
        virtual void EnqueueCommand(const std::shared_ptr<CommandInterface>& command) = 0;

        virtual void EnqueueCommandRequest(
            const std::shared_ptr<swganh::object::Object>& object,
            swganh::messages::controllers::CommandQueueEnqueue* message) = 0;

        /**
         * Sends a command queue removal message to the specified controller client.
         *
         * @param controller The controller to send the message to.
         * @param action_counter The counter associated with the command being removed.
         * @Param default_time_sec The default time the command took to process in seconds.
         * @param error An error status flag.
         * @param action An action flag associated with the error.
         */
        virtual void SendCommandQueueRemove(
            const std::shared_ptr<swganh::object::Object>& controller,
            uint32_t action_counter,
            float default_time_sec,
            uint32_t error,
            uint32_t action) = 0;
        
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
        virtual boost::optional<const CommandProperties&> FindPropertiesForCommand(swganh::HashString command) = 0;
        
        virtual void SetDefaultCommand(uint64_t queue_owner_id, const std::shared_ptr<swganh::command::CommandInterface>& command) = 0;
        
        virtual bool HasDefaultCommand(uint64_t queue_owner_id) = 0;

        virtual void ClearDefaultCommand(uint64_t queue_owner_id) = 0;
    };

}}  // namespace swganh::command
