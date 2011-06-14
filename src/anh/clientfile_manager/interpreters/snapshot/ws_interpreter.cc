/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2011 The SWG:ANH Team */

#include "ws_interpreter.h"

#include "../../iff/iff.h"
#include "../../iff/filenode.h"

using namespace std;
using namespace std::tr1::placeholders;
using namespace anh::clientfile_manager::interpreters;

ws_interpreter::ws_interpreter()
	: interpreter_interface()
{
}

void ws_interpreter::interpret_folder(std::shared_ptr<anh::clientfile_manager::iff::folder_node> node)
{
}

void ws_interpreter::interpret_data(std::shared_ptr<anh::clientfile_manager::iff::file_node> node)
{
	const std::string& nameRef = node->name();
	if(nameRef == "0000DATA")
	{
		_handle0000DATA(node->data());
	}
	else if(nameRef == "OTNL")
	{
		_handleOTNL(node->data());
	}
}

void ws_interpreter::_handleOTNL(anh::ByteBuffer& buffer)
{

	uint32_t count = buffer.read<uint32_t>();
	for(uint32_t i = 0; i < count; ++i)
	{
		names.push_back(std::move(buffer.read<std::string>(false,true)));
	}
}

void ws_interpreter::_handle0000DATA(anh::ByteBuffer& buffer)
{
	CHUNK c;

	c.id = buffer.read<uint32_t>();
	c.parent_id = buffer.read<uint32_t>();
	c.name_id = buffer.read<uint32_t>();
	c.scale = buffer.read<float>();
	c.orientation.w = buffer.read<float>();
	c.orientation.x = buffer.read<float>();
	c.orientation.y = buffer.read<float>();
	c.orientation.z = buffer.read<float>();
	c.location.x = buffer.read<float>();
	c.location.y = buffer.read<float>();
	c.location.z = buffer.read<float>();
	c.object_priority = buffer.read<float>();
	c.pob_crc = buffer.read<uint32_t>();

	chunks.insert(std::make_pair<std::uint64_t, CHUNK>(c.parent_id, std::move(c)));
}

void ws_interpreter::debug()
{
	printf("World Object Count = %d\n", chunks.size());

	CHUNK& c = chunks[0];
	printf("Id = %d\n", c.id);
	printf("Parent Id = %d\n", c.parent_id);
	printf("Name id = %d\n", c.name_id);
	printf("Name = %s\n", names[c.name_id].c_str());
	printf("Scale = %f\n", c.scale);

	printf("Orientation(w=%f,x=%f,y=%f,z=%f)\n", c.orientation.w, c.orientation.x, c.orientation.y, c.orientation.z);
	printf("Location(x=%f,y=%f,z=%f)\n", c.location.x, c.location.y, c.location.z);

	printf("Priority = %f\n", c.object_priority);
	printf("POB CRC = %02X\n", c.pob_crc);

}