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

		virtual float Process(float x, float z, float transform_value, float& base_value, std::map<uint32_t,Fractal*>& fractals);

	private:
		float minHeight, maxHeight;
	};

}
}


#endif