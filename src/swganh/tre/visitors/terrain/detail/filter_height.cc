#include "filter_height.h"

using namespace swganh::tre;

void HeightFilter::Deserialize(anh::ByteBuffer& buffer)
{
	minHeight = buffer.read<float>();
	maxHeight = buffer.read<float>();

	feather_type = buffer.read<uint32_t>();
	feather_amount = buffer.read<float>();
}

double HeightFilter::Process(double x, double z, double transform_value, double& base_value, std::vector<Fractal>& fractals)
{
	double result;

	if ((base_value > minHeight) && (base_value < maxHeight)) {
		double feather_result = (maxHeight - minHeight) * feather_amount * 0.5;

		if (minHeight + feather_result <= base_value) {
			if (maxHeight - feather_result >= base_value) {
				result = 1.0;
			} else
				result = (maxHeight - base_value) / feather_result;

		} else {
			result = (base_value - minHeight) / feather_result;
		}
	} else
		result = 0;

	return result;
}