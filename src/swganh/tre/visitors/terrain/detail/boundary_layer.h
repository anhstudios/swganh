#pragma once

#include "swganh/byte_buffer.h"
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
		
		uint32_t feather_type;
		float feather_amount;
	};
}
}
