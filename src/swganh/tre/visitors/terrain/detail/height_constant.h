#ifndef HEIGHT_CONSTANT_H_
#define HEIGHT_CONSTANT_H_

#include "height_layer.h"

namespace swganh
{
namespace tre
{
	class HeightConstant : public HeightLayer
	{
	public:

		virtual void Deserialize(anh::ByteBuffer& buffer);
		
		virtual void GetBaseHeight(double x, double z, double transform_value, double& base_value, std::vector<Fractal>& fractals);

	private:
		int   fractal_id;
		int   transform_type;
		double height_val;
	};

}
}


#endif