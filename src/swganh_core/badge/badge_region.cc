// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include <swganh_core/badge/badge_region.h>
#include <swganh/connection/connection_client_interface.h>

#include <swganh_core/messages/chat_system_message.h>
#include <swganh_core/messages/play_music_message.h>
#include <swganh_core/messages/out_of_band.h>
#include <swganh_core/messages/system_message.h>

#include "badge_service.h"

using namespace swganh::object::regions;
using namespace swganh::badge;
using namespace swganh::messages;

BadgeRegion::BadgeRegion(std::string badge_name, BadgeService* service)
	: badge_service_(service)
	, badge_name_(badge_name)
{
	SetCollidable(true);
}

BadgeRegion::~BadgeRegion(void)
{
}

void BadgeRegion::OnCollisionEnter(std::shared_ptr<swganh::object::Object> collider)
{
	badge_service_->GiveBadge(collider, badge_name_);
}

void BadgeRegion::__BuildCollisionBox(void)
{
	boost::geometry::append(local_collision_box_, Point(-25, -25));
	boost::geometry::append(local_collision_box_, Point(-25, 25));
	boost::geometry::append(local_collision_box_, Point(25, 25));
	boost::geometry::append(local_collision_box_, Point(25, -25));
}