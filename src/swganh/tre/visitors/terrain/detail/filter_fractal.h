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
		
		virtual void Deserialize(anh::ByteBuffer& buffer);

		virtual double Process(double x, double z, double transform_value, double& base_value, std::vector<Fractal>& fractals);

	private:
		uint32_t fractal_id;
		double min, max, step;
	};

}
}


#endif