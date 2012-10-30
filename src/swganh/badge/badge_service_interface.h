// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <swganh/service/service_interface.h>

namespace swganh {

namespace object {
	class Object;
} // namespace swganh::object

namespace badge {
	class BadgeServiceInterface : public swganh::service::ServiceInterface
	{
	public:
		virtual void GiveBadge(std::shared_ptr<swganh::object::Object> player, std::string name)=0;
		virtual void GiveBadge(std::shared_ptr<swganh::object::Object> player, uint32_t id)=0;

		virtual bool HasBadge(std::shared_ptr<swganh::object::Object> object, std::string name)=0;
		virtual bool HasBadge(std::shared_ptr<swganh::object::Object> object, uint32_t id)=0;
	};
}} // namespace swganh::badge