#include "height_fractal.h"
#include "fractal.h"

using namespace swganh::tre;

void HeightFractal::Deserialize(anh::ByteBuffer& buffer)
{
	this->fractal_id = buffer.read<uint32_t>();
	this->transform_type = buffer.read<uint32_t>();
	this->height_val = buffer.read<float>();
}
		
void HeightFractal::GetBaseHeight(double x, double z, double transform_value, double& base_value, std::vector<Fractal>& fractals)
{
	double noise_result = fractals[fractal_id].getNoise(x, z) * height_val;

	double result;

	switch (transform_type)
	{
	case 1:
		result = base_value + noise_result * transform_value;
		break;
	case 2:
		result = base_value - noise_result * transform_value;
		break;
	case 3:
		result = base_value + (noise_result * base_value - base_value) * transform_value;
		break;
	case 4:
		result = base_value;
		break;
	default:
		result = base_value + (noise_result - base_value) * transform_value;
		break;
	}

	base_value = result;
}