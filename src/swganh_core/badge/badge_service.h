// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include <bitset>

#include <swganh/app/swganh_kernel.h>
#include <swganh/badge/badge_service_interface.h>
#include <swganh/connection/connection_client_interface.h>

namespace swganh {

namespace command {
	class CommandServiceInterface;
} // namespace swganh::command

namespace badge {

	enum BadgeType
	{
		ACCUMULATION = 1,
		INTEREST,
		EXPLORATION_JEDI,
		EXPLORATION_DANGEROUS,
		EXPLORATION_EASY,
		EVENTS,
		CONTENT,
		MASTER
	};

	struct Badge
	{
		uint32_t id;
		std::string name;
		std::string sound;
		BadgeType type;
	};

	class BadgeService : public BadgeServiceInterface
	{
	public:
		BadgeService(swganh::app::SwganhKernel* kernel);
		~BadgeService();

		void Startup();

		swganh::service::ServiceDescription GetServiceDescription();

		virtual void GiveBadge(std::shared_ptr<swganh::object::Object> object, std::string name);
		virtual void GiveBadge(std::shared_ptr<swganh::object::Object> object, uint32_t id);

		swganh::app::SwganhKernel* kernel_;
		swganh::command::CommandServiceInterface* command_service_;

	private:
		std::vector<Badge> badges_;
	};

}} // namespace swganh::badge