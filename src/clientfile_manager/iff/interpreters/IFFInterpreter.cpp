#include "IFFInterpreter.h"

#include <anh/byte_buffer.h>

std::string ClientFileManager::IFF::Interpreters::IFFInterpreter::readStringFromByteBuffer(anh::ByteBuffer& b)
{
	std::string s;
	char buf = b.read<char>();
	while(buf != 0)
	{
		s.push_back(buf);
		buf = b.read<char>();
	}
	s.push_back(0);
	return s;
}