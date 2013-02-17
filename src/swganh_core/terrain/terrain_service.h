// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#pragma once

#include "swganh_core/terrain/terrain_service_interface.h"
#include "swganh/app/swganh_kernel.h"

#include <boost/thread/mutex.hpp>
#include <map>
#include <list>
#include <memory>

namespace swganh
{
namespace tre
{
	class TerrainVisitor;
	//class LayerVisitor;
	class Fractal;

	class ContainerLayer;
}
}


namespace swganh
{


namespace terrain
{
	struct LayerEntry
	{
		//std::shared_ptr<swganh::tre::LayerVisitor> layer_visitor_;
		float offset_x, offset_z;
	};

	struct SceneEntry
	{
		std::shared_ptr<swganh::tre::TerrainVisitor> terrain_visitor_;
		//std::list<LayerEntry> layers_;
	};

	typedef std::map<uint32_t, SceneEntry> SceneMap;

	class TerrainService : public swganh::terrain::TerrainServiceInterface
	{
	public:

		TerrainService(swganh::app::SwganhKernel* kernel);

		virtual float GetWaterHeight(uint32_t scene_id, float x, float z, float raw=false);

		virtual float GetHeight(uint32_t scene_id, float x, float z, bool raw=false);

		virtual bool IsWater(uint32_t scene_id, float x, float z, bool raw=false);

		swganh::service::ServiceDescription GetServiceDescription();
        

	private:

		bool waterHeightHelper(swganh::tre::ContainerLayer* layer, float x, float z, float& result);

		float processLayerHeight(swganh::tre::ContainerLayer* layer, float x, float z, float& base_value, float affector_transform, std::map<uint32_t, swganh::tre::Fractal*>& fractals);
		float calculateFeathering(float value, int featheringType);

		boost::mutex terrain_mutex_;
		SceneMap scenes_;
		swganh::app::SwganhKernel* kernel_;
	};
}
}
