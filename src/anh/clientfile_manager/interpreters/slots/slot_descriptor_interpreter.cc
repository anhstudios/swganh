/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2011 The SWG:ANH Team */

#include "slot_descriptor_interpreter.h"

#include "../../iff/filenode.h"

using namespace std;
using namespace anh::clientfile_manager::interpreters;

void slot_descriptor_interpreter::interpret(shared_ptr<anh::clientfile_manager::iff::file_node> node)
{
	if(node->name() == "0000DATA")
	{
		_handle0000DATA(node->data());
	}
}

void slot_descriptor_interpreter::_handle0000DATA(anh::ByteBuffer& buf)
{
	while(buf.read_position() < buf.size())
	{
		slots_available.push_back(buf.read<std::string>(false, true));
	}
}