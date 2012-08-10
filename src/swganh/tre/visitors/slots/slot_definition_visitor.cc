// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "slot_definition_visitor.h"

#include "../../iff/iff.h"
#include "../../iff/filenode.h"
#include "../../iff/foldernode.h"

using namespace std;
using namespace swganh::tre;

void SlotDefinitionVisitor::visit_folder(std::shared_ptr<folder_node> node)
{
}

void SlotDefinitionVisitor::visit_data(shared_ptr<file_node> node)
{
	if(node->name() == "0006DATA")
	{
		_handle0006DATA(node->data());
	}
}

void SlotDefinitionVisitor::_handle0006DATA(anh::ByteBuffer& buf)
{
	while(buf.read_position() < buf.size())
	{
		slot_entry entry;

		entry.name = buf.read<std::string>(false, true);
		
		entry.global = buf.read<char>() != 0;
		entry.canMod = buf.read<char>() != 0;
		entry.exclusive = buf.read<char>() != 0;

		entry.hardpoint_name = buf.read<std::string>(false, true);
		entry.unkValue = buf.read<std::uint32_t>();

		slots_.push_back(std::move(entry));
	}
}

size_t SlotDefinitionVisitor::findSlotByName(const std::string& name)
{
	for(int i=0; i < slots_.size(); ++i) {
	{
		if(slots_[i].name == name)
			return i;
	}
	return -1;
}