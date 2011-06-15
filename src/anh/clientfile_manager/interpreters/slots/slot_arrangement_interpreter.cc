/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2011 The SWG:ANH Team */

#include <anh/clientfile_manager/interpreters/slots/slot_arrangement_interpreter.h>

#include <anh/clientfile_manager/iff/filenode.h>
#include <anh/clientfile_manager/iff/foldernode.h>

using namespace std;
using namespace anh::clientfile_manager::interpreters;

void slot_arrangement_interpreter::interpret_folder(std::shared_ptr<anh::clientfile_manager::iff::folder_node> node)
{
}

void slot_arrangement_interpreter::interpret_data(shared_ptr<anh::clientfile_manager::iff::file_node> node)
{
	if(node->name() == "0000ARG ")
	{
		_handle0000ARG(node->data());
	}
}

void slot_arrangement_interpreter::_handle0000ARG(anh::ByteBuffer& buf)
{
	std::vector<string> combination;
	while(buf.read_position() < buf.size())
	{
		combination.push_back(buf.read<string>(false, true));
	}
	combinations_.push_back(std::move(combination));
}