#ifndef BOUNDARY_POLYLINE_H_
#define BOUNDARY_POLYLINE_H_

#include "boundary_layer.h"
#include <glm/glm.hpp>

namespace swganh
{
namespace tre
{

	class BoundaryPolyline : public BoundaryLayer
	{
	public:

		virtual void Deserialize(anh::ByteBuffer& buffer);
		virtual bool IsContained(double px, double pz);
		virtual double Process(double px, double pz);
		
	protected:
		std::vector<glm::vec2> verts;
		double line_width;
		double max_x, max_z, min_x, min_z;
	};

}
}


#endif