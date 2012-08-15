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

		virtual bool IsContained(float px, float pz);
		virtual float Process(float px, float pz);
		
	protected:
		std::vector<glm::vec2> verts;
		int use_water_height;
		float water_height;
		float water_shader_size;
		std::string water_shader;
		float max_x, max_z, min_x, min_z;
	};

}
}


#endif