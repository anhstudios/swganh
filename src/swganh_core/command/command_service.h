// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <tuple>

#include <boost/optional.hpp>

#include "swganh/event_dispatcher.h"

#include "swganh_core/command/command_service_interface.h"

namespace swganh {
namespace observer {
	class ObserverInterface;
}
}

namespace swganh {
namespace command {

	/**
	* The command service builds, checks, queues, and runs commands.
	*/
    class CommandService: public swganh::command::CommandServiceInterface
    {
    public:

		/**
		* Creates a new instance
		*/
        explicit CommandService(swganh::app::SwganhKernel* kernel);

		/**
		* @return the service description for this service
		*/
        swganh::service::ServiceDescription GetServiceDescription();

		/**
		* Adds a new filter that is checked before enqueue
		* @param filter the filter to add
		*/
        void AddCommandEnqueueFilter(swganh::command::CommandFilter&& filter);
        
		/**
		* Adds a new filter that is checked before processing
		* @param filter the filter to add
		*/
        void AddCommandProcessFilter(swganh::command::CommandFilter&& filter);
        
		/**
		* Adds a new command creation source
		* @param command the name of the command
		* @param creator the object that will create the command.
		*/
        void AddCommandCreator(swganh::HashString command, swganh::command::CommandCreator&& creator);

		/**
		* Removes a command creator by name
		* @param command the name of the command to remove.
		*/
        void RemoveCommandCreator(swganh::HashString command);
        
		/**
		* Creates a new command by name.
		* @param command the name of the command to create
		*/
        std::shared_ptr<swganh::command::CommandInterface> CreateCommand(swganh::HashString command);

		/**
		* Enqueues a command
		* @param command the command to enqueue
		*/
        void EnqueueCommand(const std::shared_ptr<swganh::command::CommandInterface>& command);

		/**
		* Sends the command queue removal message
		* @param object object receiving the message
		* @param action_counter the counter for the action. +1 each time
		* @param default_time_sec the number of seconds the action takes to execute
		* @param error the error to send
		* @param action sets either state or locomotion depending on the error
		*/
        void SendCommandQueueRemove(
            const std::shared_ptr<swganh::object::Object>& object,
            uint32_t action_counter,
            float default_time_sec,
            uint32_t error,
            uint32_t action);
        
		/**
		* Runs the command through a series of filters in prep for enqueue
		* @param command the command to validate
		* @return a tuple representing success or failure.
		*/
        std::tuple<bool, uint32_t, uint32_t> ValidateForEnqueue(swganh::command::CommandInterface* command);
        
		/**
		* Runs the command through a series of filters in prep for processing
		* @param command the command to validate
		* @return a tuple representing success or failure.
		*/
        std::tuple<bool, uint32_t, uint32_t> ValidateForProcessing(swganh::command::CommandInterface* command);
        
		/**
		* Loads the command properties map and returns it
		*/
        swganh::command::CommandPropertiesMap LoadCommandPropertiesMap();

		/**
		* Tries to find properities for a particular command.
		*/
        boost::optional<const swganh::command::CommandProperties&> FindPropertiesForCommand(swganh::HashString command);

		/**
		* Called on startup
		*/
        void Startup();
        
		/**
		* Handler for the EnqueueCommand message.
		* @param controller the object that requested the enqueue
		* @param message the command queue enqueue message received
		*/
        void EnqueueCommandRequest(
            const std::shared_ptr<swganh::object::Object>& controller,
            swganh::messages::controllers::CommandQueueEnqueue* message);
        
        /**
		* Sets the default command for an object_id
		* @param queue_owner_id the id of the object to set the default command for
		* @param command the new default command
		*/
        void SetDefaultCommand(uint64_t queue_owner_id, const std::shared_ptr<swganh::command::CommandInterface>& command);

		/**
		* Clears the default command for an object by id
		* @param queue_owner_id the id of the object to clear
		*/
        void ClearDefaultCommand(uint64_t queue_owner_id);

		/**
		* Checks the default command for an object by id
		* @param queue_owner_id the id of the object to check
		*/
        bool HasDefaultCommand(uint64_t queue_owner_id);

    private:
        void SubscribeObjectReadyEvent(swganh::EventDispatcher* dispatcher);

        void SubscribeObjectRemovedEvent(swganh::EventDispatcher* dispatcher);
                        
        swganh::app::SwganhKernel* kernel_;
        std::shared_ptr<swganh::command::CommandFactoryInterface> command_factory_impl_;
        std::shared_ptr<swganh::command::CommandPropertiesManagerInterface> command_properties_manager_impl_;
        std::shared_ptr<swganh::command::CommandQueueManagerInterface> command_queue_manager_impl_;
        std::shared_ptr<swganh::command::CommandValidatorInterface> command_validator_impl_;
        swganh::simulation::SimulationServiceInterface* simulation_service_;
        std::string script_prefix_;
        swganh::CallbackId obj_ready_id_;
        swganh::CallbackId obj_removed_id_;
    };

}}  // namespace swganh::command
