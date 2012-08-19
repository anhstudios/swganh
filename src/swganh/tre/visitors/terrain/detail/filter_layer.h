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

		virtual double Process(double x, double z, double transform_value, double& base_value, std::vector<Fractal>& fractals) = 0;
		
	protected:
		int   feather_type;
        double feather_amount;
    };
}
}
#endif