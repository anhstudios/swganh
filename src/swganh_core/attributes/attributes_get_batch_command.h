// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <memory>

#include "swganh_core/command/base_swg_command.h"

namespace swganh {
namespace simulation {
	class SimulationServiceInterface;
}}

namespace swganh {
namespace attributes {

    class AttributesService;

	/*!
	* An implementation of a base swg command handler
	* that handles the GetAttributesBatch message.
	*/
    class GetAttributesBatchCommand : public swganh::command::BaseSwgCommand
    {
    public:

        virtual std::string GetCommandName() const { return "getattributesbatch"; }

		/*!
		* Creates a new instance using the kernel
		* and the given properties
		*/
        void Initialize(
            swganh::app::SwganhKernel* kernel,
            const swganh::command::CommandProperties& properties);

		/*!
		* Method that is called when the command is processed.
		* @Override
		*/
        virtual void Run();

    private:
        AttributesService* attributes_service_;
		swganh::simulation::SimulationServiceInterface* simulation_service_;
    };

}}
