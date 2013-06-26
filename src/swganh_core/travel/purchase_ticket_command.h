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
        virtual std::string GetCommandName() const { return "purchaseticket"; }

		virtual void Run();

	private:
		TravelService* travel_;
	};

}} // namespace swganh::travel