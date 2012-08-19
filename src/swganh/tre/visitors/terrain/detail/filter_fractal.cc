#include "filter_fractal.h"

#include "fractal.h"

using namespace swganh::tre;

void FractalFilter::Deserialize(anh::ByteBuffer& buffer)
{
	fractal_id = buffer.read<uint32_t>();
	
	feather_type = buffer.read<uint32_t>();
	feather_amount = buffer.read<float>();
	
	min = buffer.read<float>();
	max = buffer.read<float>();

	step = buffer.read<float>();
}

double FractalFilter::Process(double x, double z, double transform_value, double& base_value, std::vector<Fractal>& fractals)
{
	double noise_result = fractals[fractal_id].getNoise(x, z) * step;
	double result = 0;

	if (noise_result > min && noise_result < max) {
		double feather_result = (max - min) * feather_amount * 0.5;

		if (min + feather_result <= noise_result) {
			if (max - feather_result >= noise_result)
				result = 1.0;
			else
				result = (max - noise_result) / feather_result;
		} else
			result = (noise_result - min) / feather_result;
	} else
		result = 0;

	return result;
}