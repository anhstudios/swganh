#pragma once

#include "swganh/event_dispatcher.h"

namespace swganh_core
{
namespace simulation
{
struct NewSceneEvent : anh::BaseEvent
{
	NewSceneEvent(anh::EventType type, uint32_t scene_id_, std::string scene_label_, std::string terrain_filename_)
        : BaseEvent(type)
        , scene_id(scene_id_)
		, scene_label(scene_label_)
		, terrain_filename(terrain_filename_)
    {}
    uint32_t scene_id; 
	std::string scene_label;
	std::string terrain_filename;
};

struct DestroySceneEvent : anh::BaseEvent
{
	DestroySceneEvent(anh::EventType type, uint32_t scene_id_)
        : BaseEvent(type)
        , scene_id(scene_id_)
    {}
    uint32_t scene_id; 
};

}
}