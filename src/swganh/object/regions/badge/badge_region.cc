// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include <swganh/object/regions/badge/badge_region.h>
#include <swganh/connection/connection_client_interface.h>

#include <pub14_core/messages/chat_system_message.h>
#include <pub14_core/messages/play_music_message.h>
#include <pub14_core/messages/out_of_band.h>


using namespace swganh::object::regions;

BadgeRegion::BadgeRegion(void)
{
}

BadgeRegion::~BadgeRegion(void)
{
}

void BadgeRegion::OnCollisionEnter(std::shared_ptr<swganh::object::Object> collider)
{
	if(collider->HasController())
	{
		messages::PlayMusicMessage music_message;
		music_message.music_file = "sound/music_emperor_theme_loop.snd";
		music_message.unknown2 = 1;
		
		collider->GetController()->GetRemoteClient()->SendTo(music_message);
		collider->GetController()->SendSystemMessage(messages::OutOfBand("badge_n", "exp_cor_agrilat_swamp"));
	}
}

void BadgeRegion::__BuildCollisionBox(void)
{
	boost::geometry::append(local_collision_box_, Point(-25, -25));
	boost::geometry::append(local_collision_box_, Point(-25, 25));
	boost::geometry::append(local_collision_box_, Point(25, 25));
	boost::geometry::append(local_collision_box_, Point(25, -25));
	UpdateWorldCollisionBox();
}

void BadgeRegion::__BuildBoundingVolume(void)
{
	local_bounding_volume_ = BoundingVolume(Point(-25, -25), Point(25, 25));
	UpdateWorldBoundingVolume();
}