#ifndef FILTER_SLOPE_H_
#define FILTER_SLOPE_H_

#include "filter_layer.h"

namespace swganh
{
namespace tre
{

	class SlopeFilter : public FilterLayer
	{
	public:
		
		virtual void Deserialize(anh::ByteBuffer buffer) = 0;

		virtual float Process(float x, float z, float transform_value, float& base_value, std::vector<Fractal>& fractals);

	private:
		float min_angle;
		float min;
		float max_angle;
		float max;

		float default_value;

	};

}
}


#endif