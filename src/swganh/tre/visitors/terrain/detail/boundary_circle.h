#ifndef BOUNDARY_CIRCLE_H_
#define BOUNDARY_CIRCLE_H_

#include "boundary_layer.h"

namespace swganh
{
namespace tre
{

	class BoundaryCircle : public BoundaryLayer
	{
	public:

		virtual void Deserialize(anh::ByteBuffer& buffer);
		virtual bool IsContained(double px, double pz);
		virtual double Process(double px, double pz);
		
	protected:
		double x;
		double z;
		double rad;
	};

}
}


#endif