#ifndef BOUNDARY_RECTANGLE_H_
#define BOUNDARY_RECTANGLE_H_

#include "boundary_layer.h"

namespace swganh
{
namespace tre
{

	class BoundaryRectangle : public BoundaryLayer
	{
	public:

		virtual void Deserialize(anh::ByteBuffer& buffer);
		virtual bool IsContained(double px, double pz);
		virtual double Process(double px, double pz);
		
	protected:
		double x1, z1, x2, z2;
	};

}
}


#endif