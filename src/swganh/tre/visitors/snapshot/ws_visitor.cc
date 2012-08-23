// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "ws_visitor.h"

#ifndef WIN32
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#else
#define PRIu64       "I64u"
#endif

#include "../../iff/iff.h"
#include "../../iff/filenode.h"

using namespace std;
using namespace std::placeholders;
using namespace swganh::tre;

WsVisitor::WsVisitor()
	: VisitorInterface()
{
}

void WsVisitor::visit_folder(uint32_t depth, std::shared_ptr<folder_node> node)
{
}

void WsVisitor::visit_data(uint32_t depth, std::shared_ptr<file_node> node)
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

void WsVisitor::_handleOTNL(anh::ByteBuffer& buffer)
{

	uint32_t count = buffer.read<uint32_t>();
	for(uint32_t i = 0; i < count; ++i)
	{
		names.push_back(std::move(buffer.read<std::string>(false,true)));
	}
}

void WsVisitor::_handle0000DATA(anh::ByteBuffer& buffer)
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

	chunks_.push_back(std::move(c));
}