#include "boundary_rectangle.h"

using namespace swganh::tre;

void BoundaryRectangle::Deserialize(anh::ByteBuffer& buffer)
{
	x1 = buffer.read<float>();
	z1 = buffer.read<float>();

	x2 = buffer.read<float>();
	z2 = buffer.read<float>();

	feather_type = buffer.read<uint32_t>();
	feather_amount = buffer.read<float>();

	//Make sure x1, z1 is lower left and x2,z2 is top right
	double temp;
	if (x1 > x2)
	{
		temp = x1;
		x1 = x2;
		x2 = temp;
	}

	if (z1 > z2)
	{
		temp = z1;
		z1 = z2;
		z2 = temp;
	}
}

bool BoundaryRectangle::IsContained(double px, double pz)
{
	if (x2 >= px && x1 <= px && z2 >= pz && z1 <= pz)
		return true;

	return false;
}

double BoundaryRectangle::Process(double px, double pz)
{
	double result;
	
	if (!IsContained(px, pz))
		result = 0.0f;
	else
	{
		double min_distx = px - x1;
		double max_distx = x2 - px;
		double min_distz = pz - z1;
		double max_distz = z2 - pz;
		double x_length = x2 - x1;
		double length = z2 - z1;

		if (x_length < length)
			length = x_length;

		double feather_length = feather_amount * length * 0.5;
		double feather_result = feather_length;

		double newX0 = x1 + feather_length;
		double newX1 = x2 - feather_length;
		double newZ0 = z1 + feather_length;
		double newZ1 = z2 - feather_length;

		if (px < newX1 || px > newX0 || pz < newZ1 || pz > newZ0)
			return 1.0f;

		if (min_distx < feather_length)
			feather_result = min_distx;
		if (max_distx < feather_result)
			feather_result = max_distx;
		if (min_distz < feather_result)
			feather_result = min_distz;
		if (max_distz < feather_result)
			feather_result = max_distz;

		result = feather_result / feather_length;
	}

	return result;
}