#ifndef ANH_TERRAIN_HEIGHT_H_
#define ANH_TERRAIN_HEIGHT_H_

#include <vector>
#include "anh/byte_buffer.h"
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

		virtual void Deserialize(anh::ByteBuffer buffer) = 0;
		
		virtual void GetBaseHeight(float x, float z, float transform_value, float& base_value, std::vector<Fractal>& fractals) = 0;
	};
	
}
}
#endif
