#ifndef BOUNDARY_POLYGON_H_
#define BOUNDARY_POLYGON_H_

#include "boundary_layer.h"
#include <glm/glm.hpp>
#include <string>

namespace swganh
{
namespace tre
{

	class BoundaryPolygon : public BoundaryLayer
	{
	public:

		virtual void Deserialize(anh::ByteBuffer& buffer);
		virtual bool IsContained(double px, double pz);
		virtual double Process(double px, double pz);
		
	protected:
		std::vector<glm::vec2> verts;
		uint32_t use_water_height;
		double water_height;
		double water_shader_size;
		std::string water_shader;
		double max_x, max_z, min_x, min_z;
	};

}
}


#endif