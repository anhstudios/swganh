/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2010 The SWG:ANH Team */

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