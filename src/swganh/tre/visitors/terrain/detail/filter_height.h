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
		
		virtual void Deserialize(anh::ByteBuffer buffer) = 0;

		virtual float Process(float x, float z, float transform_value, float& base_value, std::vector<Fractal>& fractals);

	private:
		float minHeight;
		float maxHeight;
	};

}
}


#endif