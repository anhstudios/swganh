// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_ATTRIBUTES_GET_BATCH_COMMAND_H_
#define SWGANH_ATTRIBUTES_GET_BATCH_COMMAND_H_

#include <memory>

#include "swganh/command/base_swg_command.h"

namespace swganh {
namespace simulation {
	class SimulationServiceInterface;
}}

namespace swganh_core {
namespace attributes {

    class AttributesService;

    class GetAttributesBatchCommand : public swganh::command::BaseSwgCommand
    {
    public:
        GetAttributesBatchCommand(
            swganh::app::SwganhKernel* kernel,
            const swganh::command::CommandProperties& properties);

        virtual ~GetAttributesBatchCommand();

        virtual boost::optional<std::shared_ptr<swganh::command::CommandCallback>> Run();

    private:
        AttributesService* attributes_service_;
		swganh::simulation::SimulationServiceInterface* simulation_service_;
    };

}}

#endif  // SWGANH_ATTRIBUTES_GET_BATCH_COMMAND_H_
