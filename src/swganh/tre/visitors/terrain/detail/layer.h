#ifndef TERRAIN_LAYER_H_
#define TERRAIN_LAYER_H_

namespace swganh
{
namespace tre
{

	enum LayerType
	{
		LAYER_TYPE_CONTAINER = 1,
		LAYER_TYPE_BOUNDARY,
		LAYER_TYPE_HEIGHT,
		LAYER_TYPE_FILTER,
	};

	class Layer
	{
	public:

		virtual LayerType GetType() = 0;
		
		virtual void Deserialize(anh::ByteBuffer buffer) = 0;

	};

}
}

#endif