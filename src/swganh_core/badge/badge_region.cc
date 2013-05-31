// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include <swganh_core/badge/badge_region.h>
#include <swganh_core/connection/connection_client_interface.h>

#include <swganh_core/messages/chat/chat_system_message.h>
#include <swganh_core/messages/play_music_message.h>
#include <swganh_core/messages/out_of_band.h>
#include <swganh_core/messages/system_message.h>

#include "badge_service.h"

using namespace swganh::badge;
using namespace swganh::messages;
using swganh::object::Object;

BadgeRegion::BadgeRegion(std::string badge_name, BadgeService* service)
	: badge_service_(service)
	, badge_name_(badge_name)
{
	SetCollidable(true);
	SetDatabasePersisted(false);
	SetInSnapshot(true);
}

BadgeRegion::~BadgeRegion(void)
{
}

void BadgeRegion::OnCollisionEnter(std::shared_ptr<Object> collider)
{
    if(collider->HasController())
    {
    	badge_service_->GiveBadge(collider, badge_name_);
    } else {
        collider->ViewObjects(nullptr, 0, true, [this] (const std::shared_ptr<Object>& inner)
        {
            if(inner->HasController())
            {
    	        badge_service_->GiveBadge(inner, badge_name_);
            }
        });
    }
}