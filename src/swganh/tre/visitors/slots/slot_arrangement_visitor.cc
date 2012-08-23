// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "slot_arrangement_visitor.h"

#include "../../iff/iff.h"
#include "../../iff/filenode.h"
#include "../../iff/foldernode.h"

using namespace std;
using namespace swganh::tre;

void SlotArrangementVisitor::visit_folder(uint32_t depth, std::shared_ptr<folder_node> node)
{
}

void SlotArrangementVisitor::visit_data(uint32_t depth, shared_ptr<file_node> node)
{
	if(node->name() == "0000ARG ")
	{
		_handle0000ARG(node->data());
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