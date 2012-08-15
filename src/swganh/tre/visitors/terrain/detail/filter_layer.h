#ifndef ANH_TERRAIN_FILTER_H_
#define ANH_TERRAIN_FILTER_H_

#include <vector>
#include "anh/byte_buffer.h"
#include "layer.h"

namespace swganh
{
namespace tre
{
	class Fractal;

    class FilterLayer : public Layer
    {
    public:

		virtual LayerType GetType() { return LAYER_TYPE_FILTER; }

		virtual void Deserialize(anh::ByteBuffer buffer) = 0;

		virtual float Process(float x, float z, float transform_value, float& base_value, std::vector<Fractal>& fractals) = 0;
		
	protected:
		int   feather_type;
        float feather_amount;
    };
}
}
#endif