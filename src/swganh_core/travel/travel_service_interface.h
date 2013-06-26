// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <swganh/service/service_interface.h>

namespace swganh {
namespace object {
	class Object;
} // namespace swganh::object

namespace travel {
	class TravelServiceInterface : public swganh::service::BaseService
	{
	public:

        virtual ~TravelServiceInterface() {}

		/**
		 * Begins the ticket transaction processes by opening the ticket purchasing window.
		 * 
		 * @param object Target player.
		 */
		virtual void BeginTicketTransaction(std::shared_ptr<swganh::object::Object> object)=0;

		/**
		 * Request transportation.
		 *
		 * @param object Requestor.
		 * @param ticket Ticket inventory item.
		 */
		virtual void UseTicket(std::shared_ptr<swganh::object::Object> requester, std::shared_ptr<swganh::object::Object> ticket)=0;

		virtual std::vector<std::string> GetAvailableTickets(std::shared_ptr<swganh::object::Object> object)=0;
		virtual std::shared_ptr<swganh::object::Object> GetAvailableInventoryTicketBySelection(std::shared_ptr<swganh::object::Object> object, uint32_t selection)=0;
		virtual std::shared_ptr<swganh::object::Object> GetInventoryTicket(std::shared_ptr<swganh::object::Object> object, uint32_t index)=0;
	};
}} // namespace swganh::travel