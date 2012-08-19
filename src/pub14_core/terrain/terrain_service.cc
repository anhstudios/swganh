#include "terrain_service.h"

#include "swganh/tre/visitors/terrain/layer_visitor.h"
#include "swganh/tre/visitors/terrain/terrain_visitor.h"

#include "swganh/tre/visitors/terrain/detail/container_layer.h"
#include "swganh/tre/visitors/terrain/detail/boundary_layer.h"
#include "swganh/tre/visitors/terrain/detail/height_layer.h"
#include "swganh/tre/visitors/terrain/detail/filter_layer.h"

using namespace swganh_core::terrain;
using namespace swganh::tre;

float TerrainService::GetHeight(uint32_t scene_id, float x, float z, bool raw)
{
	auto itr = scenes_.find(scene_id);
	if(itr != scenes_.end())
	{
		float affector_transform = 1.0f;
		float transform_value = 0.0f;
		float height_result = 0.0f;
		
		//Read in height at this point



		if(!raw)
		{
			//Apply any necessary layer modifications
		}
	}
	return FLT_MIN;
}

bool TerrainService::IsWater(uint32_t scene_id, float x, float z, bool raw)
{
	auto itr = scenes_.find(scene_id);
	if(itr != scenes_.end())
	{
		//Read in height at this point

		if(!raw)
		{
			//Apply any necessary layer modifications
		}
	}
	return false;
}

float TerrainService::processLayerHeight(ContainerLayer* layer, float x, float z, float& base_value, float affector_transform)
{
	std::vector<BoundaryLayer*> boundaries = layer->boundaries;
	std::vector<HeightLayer*> heights = layer->heights;
	std::vector<FilterLayer*> filters = layer->filters;

	float transform_value = 0.0f;
	bool has_boundaries = false;
	float result = 0.0f;

	for (unsigned int i = 0; i < boundaries.size(); i++)
	{
		TRNLib::Boundary* boundary = (Boundary*)boundaries.at(i);

		if (!boundary->enabled)
			continue;
		else
			has_boundaries = true;

		float result = boundary->process(x, z);

		result = calculateFeathering(result, boundary->feather_type);

		if (result > transform_value)
			transform_value = result;

		if (transform_value >= 1)
			break;
	}

	if (has_boundaries == false)
		transform_value = 1.0f;

	if (layer->invert_boundaries)
		transform_value = 1.0f - transform_value;

	if (transform_value != 0) 
	{
		for (unsigned int i = 0; i < filters.size(); ++i) 
		{
			TRNLib::Filter* filter = (Filter*)filters.at(i);

			if (!filter->enabled)
				continue;

			float result = filter->process(x, z, transform_value, base_value, this);

			result = calculateFeathering(result, filter->feather_type);

			if (transform_value > result)
				transform_value = result;

			if (transform_value == 0)
				break;
		}

		if (layer->invert_filters)
			transform_value = 1.0 - transform_value;

		if (transform_value != 0)
		{
			for (unsigned int i = 0; i < heights.size(); i++)
			{
				TRNLib::Height* affector = (Height*)heights.at(i);
			
				if (affector->enabled)
				{
					affector->getBaseHeight(x, z, transform_value, base_value, this);
				}
			}

			std::vector<CONTAINER_LAYER*> children = layer->children;

			for (unsigned int i = 0; i < children.size(); i++)
			{
				CONTAINER_LAYER* child = children.at(i);

				if (child->enabled)
					processLayerHeight(child, x, z, base_value, affector_transform * transform_value);
			}
		}
	}

	return transform_value;
}

float TerrainService::calculateFeathering(float value, int featheringType) {
	float result = value;

	switch (featheringType) {
	case 1:
		result = result * result;
		break;
	case 2:
		result = sqrt(result);
		break;
	case 3:
		result = result * result * (3 - 2 * result);
		break;
	case 0:
		result = result;
		break;
	default:
		result = 0;
		break;
	}

	return result;
}