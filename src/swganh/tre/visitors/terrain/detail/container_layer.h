#ifndef CONTAINER_LAYER_H_
#define CONTAINER_LAYER_H_

#include "layer.h"
#include <vector>

namespace swganh
{
namespace tre
{

	class ContainerLayer : public Layer
	{
	public:

		virtual LayerType GetType() { return LAYER_TYPE_CONTAINER; }

		std::vector<Layer> sub_layers;

	private:
	};

}
}


#endif