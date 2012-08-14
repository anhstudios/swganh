// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "slot_descriptor_visitor.h"

#include "../../iff/iff.h"
#include "../../iff/filenode.h"
#include "../../iff/foldernode.h"

using namespace std;
using namespace swganh::tre;

void SlotDescriptorVisitor::visit_folder(std::shared_ptr<folder_node> node)
{
}

void SlotDescriptorVisitor::visit_data(shared_ptr<file_node> node)
{
	if(node->name() == "0000DATA")
	{
		_handle0000DATA(node->data());
	}
}

void SlotDescriptorVisitor::_handle0000DATA(anh::ByteBuffer& buf)
{
	while(buf.read_position() < buf.size())
	{
		slots_available.push_back(buf.read<std::string>(false, true));
	}
}