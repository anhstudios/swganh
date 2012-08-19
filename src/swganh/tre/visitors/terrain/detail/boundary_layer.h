#ifndef ANH_TERRAIN_BOUNDARY_H_
#define ANH_TERRAIN_BOUNDARY_H_

#include "anh/byte_buffer.h"
#include "layer.h"

namespace swganh
{
namespace tre
{
	class BoundaryLayer : public Layer
	{
	public:
		
		virtual LayerType GetType() { return LAYER_TYPE_BOUNDARY; }

		virtual bool IsContained(double px, double pz) = 0;
		virtual double Process(double px, double pz) = 0;
		
	protected:
		uint32_t feather_type;
		double feather_amount;
	};
}
}

#endif