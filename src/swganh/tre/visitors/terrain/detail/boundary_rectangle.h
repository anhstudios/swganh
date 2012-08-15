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

		virtual bool IsContained(float px, float pz);
		virtual float Process(float px, float pz);
		
	protected:
		float x1;
		float z1;

		float x2;
		float z2;
	};

}
}


#endif