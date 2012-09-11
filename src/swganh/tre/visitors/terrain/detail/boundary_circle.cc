#include "boundary_circle.h"

using namespace swganh::tre;

void BoundaryCircle::Deserialize(swganh::ByteBuffer& buffer)
{
	this->x = buffer.read<float>();
	this->z = buffer.read<float>();
	this->rad = buffer.read<float>();

	this->feather_type = buffer.read<uint32_t>();
	this->feather_amount = buffer.read<float>();
}

bool BoundaryCircle::IsContained(float px, float pz)
{
	float dist = pow(px-x,2) + pow(pz-z,2);
	float r2 = pow(rad,2);

	//std::cout << "BCIR(" << this << ")::ISCONTAINED("<< px << "," << pz << "=" << (dist < r2) <<")" << std::endl;

	if ( dist < r2)
		return true;

	return false;
}

float BoundaryCircle::Process(float px, float pz)
{
	float result;
	float dist = pow(px-x,2) + pow(pz-z,2);
	float r2 = pow(rad,2);

	if (dist <= r2)
	{
		float fCircle = (float) pow((1.0 - feather_amount) * rad,2);

		if (dist > fCircle)
			result = 1.0f - (dist - fCircle) / (r2 - fCircle);
		else
			result = 1.0f;
	}
	else
		result = 0.0f;

	//std::cout << "BCIR(" << this << ")::PROCESS("<< px << "," << pz << "=" << result <<")" << std::endl;

	return result;
}