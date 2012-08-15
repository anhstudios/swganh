#ifndef ANH_TERRAIN_BOUNDARY_H_
#define ANH_TERRAIN_BOUNDARY_H_

#include "anh/byte_buffer.h"
#include "layer.h"

namespace swganh
{
namespace tre
{
	class BoundaryLayer : public Layer
	{
	public:
		
		virtual LayerType GetType() { return LAYER_TYPE_BOUNDARY; }

		virtual bool IsContained(float px, float pz) = 0;
		virtual float Process(float px, float pz) = 0;
		
	protected:
		int feather_type;
		float feather_amount;
	};
}
}

#endif