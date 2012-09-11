#pragma once

#include "swganh/service/service_interface.h"

namespace swganh
{
namespace terrain
{
	class TerrainServiceInterface : public swganh::service::ServiceInterface
	{
	public:

		virtual float GetHeight(uint32_t scene_id, float x, float z, bool raw=false) = 0;

		virtual float GetWaterHeight(uint32_t scene_id, float x, float z, float raw=false) = 0;

		virtual bool IsWater(uint32_t scene_id, float x, float z, bool raw=false) = 0;

	};
}
}
