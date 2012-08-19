#include "height_constant.h"

using namespace swganh::tre;

void HeightConstant::Deserialize(anh::ByteBuffer& buffer)
{
	this->transform_type = buffer.read<uint32_t>();
	this->height_val = buffer.read<float>();
}
		
void HeightConstant::GetBaseHeight(double x, double z, double transform_value, double& base_value, std::vector<Fractal>& fractals)
{
	if (transform_value == 0)
		return;

	double result;

	switch (transform_type) 
	{
	case 1:
		result = transform_value * height_val + base_value;
		break;
	case 2:
		result = base_value - transform_value * height_val;
		break;
	case 3:
		result = base_value + (base_value * height_val - base_value) * transform_value;
		break;
	case 4:
		result = 0;
		break;
	default:
		result = (1.0 - transform_value) * base_value + transform_value * height_val;
	}

	base_value = result;
}