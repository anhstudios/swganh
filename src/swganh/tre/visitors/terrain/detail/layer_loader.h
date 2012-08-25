#pragma once

#include <string>


namespace swganh
{
namespace tre {
	class Layer;
}
}

swganh::tre::Layer* LayerLoader(std::string node_name);
