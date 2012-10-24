// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include <swganh_core/object/regions/badge/badge_region.h>
#include <swganh/connection/connection_client_interface.h>

#include <swganh_core/messages/chat_system_message.h>
#include <swganh_core/messages/play_music_message.h>
#include <swganh_core/messages/out_of_band.h>


using namespace swganh::object::regions;

BadgeRegion::BadgeRegion(void)
{
	SetCollidable(true);
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
		music_message.channel = 0;

		messages::ChatSystemMessage system_message;
		system_message.out_of_band = L"badge_n";
		system_message.message = L"exp_cor_agrilat_swamp";
		
		
		collider->GetController()->Notify(&music_message);
		collider->GetController()->Notify(&system_message);
	}
}

void BadgeRegion::__BuildCollisionBox(void)
{
	boost::geometry::append(local_collision_box_, Point(-25, -25));
	boost::geometry::append(local_collision_box_, Point(-25, 25));
	boost::geometry::append(local_collision_box_, Point(25, 25));
	boost::geometry::append(local_collision_box_, Point(25, -25));
}