#ifndef SWGANH_TERRAIN_SERVICE_INTERFACE_H_
#define SWGANH_TERRAIN_SERVICE_INTERFACE_H_

#include "anh/service/service_interface.h"

namespace swganh
{
namespace terrain
{
	class TerrainServiceInterface : public anh::service::ServiceInterface
	{
	public:

		virtual float GetHeight(uint32_t scene_id, float x, float z, bool raw=false) = 0;

		virtual bool IsWater(uint32_t scene_id, float x, float z, bool raw=false) = 0;

	};
}
}

#endif;