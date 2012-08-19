#ifndef FILTER_HEIGHT_H_
#define FILTER_HEIGHT_H_

#include "filter_layer.h"

namespace swganh
{
namespace tre
{

	class HeightFilter : public FilterLayer
	{
	public:
		
		virtual void Deserialize(anh::ByteBuffer& buffer);

		virtual double Process(double x, double z, double transform_value, double& base_value, std::vector<Fractal>& fractals);

	private:
		double minHeight, maxHeight;
	};

}
}


#endif