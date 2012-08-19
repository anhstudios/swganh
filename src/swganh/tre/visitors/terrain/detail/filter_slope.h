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
		
		virtual void Deserialize(anh::ByteBuffer& buffer);

		virtual double Process(double x, double z, double transform_value, double& base_value, std::vector<Fractal>& fractals);

		void SetMinAngle(double new_angle);
		void SetMaxAngle(double new_angle);

	private:
		double min_angle, min, max_angle, max, default_value;
	};

}
}


#endif