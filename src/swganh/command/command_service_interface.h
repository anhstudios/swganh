// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_COMMAND_COMMAND_SERVICE_H_
#define SWGANH_COMMAND_COMMAND_SERVICE_H_

#include <cstdint>
#include <memory>
#include <string>
#include <tuple>

#include <boost/optional.hpp>

#include "anh/service/service_interface.h"

#include "command_factory_interface.h"
#include "command_validator_interface.h"

namespace swganh {
namespace object {
    class Object;
    class ObjectController;
namespace tangible {
    class Tangible;
}
namespace creature {
	class Creature;
}} // namespace object::creature

namespace simulation {
    class SimulationService;
}}  // namespace swganh::simulation;


namespace swganh {
namespace command {

    class CommandInterface;
    class CommandQueueInterface;
    class CommandQueueManagerInterface;
    class CommandPropertiesManagerInterface;
    struct CommandProperties;

    class CommandServiceInterface: public anh::service::ServiceInterface
    {
    public:
        virtual ~CommandServiceInterface() {}

        virtual void AddCommandEnqueueFilter(CommandFilter&& filter) = 0;
        
        virtual void AddCommandProcessFilter(CommandFilter&& filter) = 0;
        
        virtual void AddCommandCreator(anh::HashString command, CommandCreator&& creator) = 0;

        virtual void RemoveCommandCreator(anh::HashString command) = 0;

        virtual void SendCommandQueueRemove(
            const std::shared_ptr<swganh::object::ObjectController>& controller,
            uint32_t action_counter,
            float default_time_sec,
            uint32_t error,
            uint32_t action) = 0;
        
        virtual std::tuple<bool, uint32_t, uint32_t> ValidateForEnqueue(CommandInterface* command) = 0;
        
        virtual std::tuple<bool, uint32_t, uint32_t> ValidateForProcessing(CommandInterface* command) = 0;

        virtual boost::optional<const CommandProperties&> FindPropertiesForCommand(anh::HashString command) = 0;
    };

}}  // namespace swganh::command

#endif  // SWGANH_COMMAND_COMMAND_SERVICE_H_
