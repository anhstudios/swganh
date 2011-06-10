#include "slot_definition_interpreter.h"

#include "../../iff/filenode.h"

using namespace std;
using namespace anh::clientfile_manager::interpreters;

void slot_definition_interpreter::interpret(shared_ptr<anh::clientfile_manager::iff::file_node> node)
{
	if(node->name() == "0006DATA")
	{
		_handle0006DATA(node->data());
	}
}

void slot_definition_interpreter::_handle0006DATA(anh::ByteBuffer& buf)
{
	while(buf.read_position() < buf.size())
	{
		slot_entry entry;

		entry.name = buf.read<std::string>(false, true);
		entry.unkFlag1 = buf.read<char>();

		//Skip the two string reading flags
		buf.read<char>();
		buf.read<char>();

		entry.hardpoint_name = buf.read<std::string>(false, true);
		entry.unkValue = buf.read<std::uint32_t>();

		slots_.push_back(std::move(entry));
	}
}