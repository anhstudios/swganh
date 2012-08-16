#ifndef FILTER_FRACTAL_H_
#define FILTER_FRACTAL_H_

#include "filter_layer.h"

namespace swganh
{
namespace tre
{

	class FractalFilter : public FilterLayer
	{
	public:
		
		virtual void Deserialize(anh::ByteBuffer buffer);

		virtual float Process(float x, float z, float transform_value, float& base_value, std::vector<Fractal>& fractals);

	private:
		uint32_t fractal_id;
		float min;
		float max;
		float step;
	};

}
}


#endif