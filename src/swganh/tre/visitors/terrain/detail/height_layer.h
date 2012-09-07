#pragma once

#include <map>
#include "swganh/byte_buffer.h"
#include "layer.h"

namespace swganh
{
namespace tre
{
	class Fractal;

	class HeightLayer : public Layer
	{
	public:

		virtual LayerType GetType() { return LAYER_TYPE_HEIGHT; }
		
		virtual void GetBaseHeight(float x, float z, float transform_value, float& base_value, std::map<uint32_t, Fractal*>& fractals) = 0;
	};
	
}
}
