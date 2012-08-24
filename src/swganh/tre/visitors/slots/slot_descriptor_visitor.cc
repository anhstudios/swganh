// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "slot_descriptor_visitor.h"

#include "../../iff/iff.h"

using namespace std;
using namespace swganh::tre;

void SlotDescriptorVisitor::visit_folder(uint32_t depth, std::string name, uint32_t size)
{
}

void SlotDescriptorVisitor::visit_data(uint32_t depth, std::string name, uint32_t size, anh::ByteBuffer& data)
{
	if(name == "0000DATA")
	{
		_handle0000DATA(data);
	}
}

void SlotDescriptorVisitor::_handle0000DATA(anh::ByteBuffer& buf)
{
	while(buf.read_position() < buf.size())
	{
		slots_available.push_back(buf.read<std::string>(false, true));
	}
}