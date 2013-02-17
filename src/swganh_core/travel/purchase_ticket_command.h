// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <memory>

#include <swganh_core/command/base_swg_command.h>

namespace swganh {
namespace travel {

	class TravelService;

	class PurchaseTicketCommand : public swganh::command::BaseSwgCommand
	{
	public:
		PurchaseTicketCommand(swganh::app::SwganhKernel* kernel,
			const swganh::command::CommandProperties& properties);

		virtual boost::optional<std::shared_ptr<swganh::command::CommandCallback>> Run();

	private:
		swganh::app::SwganhKernel* kernel_;
		TravelService* travel_;
	};

}} // namespace swganh::travel