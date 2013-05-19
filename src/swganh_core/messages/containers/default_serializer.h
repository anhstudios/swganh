// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

namespace swganh
{
namespace containers
{

template<typename T>
class DefaultSerializer
{
public:
	
	static void SerializeBaseline(swganh::ByteBuffer& message, T& t)
	{
		message.data.write<uint8_t>(0);
		message.data.write<T>(t);
	}
	
	static void SerializeDelta(swganh::ByteBuffer& message, T& t)
	{
		message.data.write<T>(t);
	}
	
};

}
}