// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "slot_arrangement_visitor.h"

#include "../../iff/iff.h"

using namespace std;
using namespace swganh::tre;

void SlotArrangementVisitor::visit_folder(uint32_t depth, std::string name, uint32_t size)
{
}

void SlotArrangementVisitor::visit_data(uint32_t depth, std::string name, uint32_t size, anh::ByteBuffer& data)
{
	if(name == "0000ARG ")
	{
		_handle0000ARG(data);
	}
}

void SlotArrangementVisitor::_handle0000ARG(anh::ByteBuffer& buf)
{
	std::vector<string> combination;
	while(buf.read_position() < buf.size())
	{
		combination.push_back(buf.read<string>(false, true));
	}
	combinations_.push_back(std::move(combination));
}