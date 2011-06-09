#include "datatable_interpreter.h"
#include "../../iff/filenode.h"

#include <regex>

using namespace std;
using namespace anh::clientfile_manager::interpreters;

std::string readDelimeterEndedString(anh::ByteBuffer& buf, char delimeter1 = 0, char delimeter2 = 0)
{
	std::string result;

	char buffer = buf.read<char>();
	while(buffer != delimeter1 && buffer != delimeter2)
	{
		result.push_back(buffer);
		buffer = buf.read<char>();
	}

	return result;
}

void datatable_interpreter::interpret(std::shared_ptr<anh::clientfile_manager::iff::file_node> node)
{
	if(node->name() == "0001COLS")
	{
		_handle0001COLS(node->data());
	}
	else if(node->name() == "TYPE")
	{
		_handleTYPE(node->data());
	}
	else if(node->name() == "ROWS")
	{
		_handleROWS(node->data());
	}
}

void datatable_interpreter::_handle0001COLS(anh::ByteBuffer& buf)
{
	std::uint32_t count = buf.read<std::uint32_t>();
	for(unsigned int i=0; i < count; ++i)
	{
		column_names_.push_back(buf.read<std::string>(false, true));
	}
}

void datatable_interpreter::_handleTYPE(anh::ByteBuffer& buf)
{
	std::string buffer;
	for(unsigned int i=0; i < column_names_.size(); ++i)
	{
		buffer = buf.read<std::string>(false, true);
		if(buffer.size() > 0)
		{
			switch(buffer[0])
			{
			case 'i':
			case 'I':
				column_types_.push_back('i');
				break;
			case 's':
			case 'S':
				column_types_.push_back('s');
				break;
			case 'f':
			case 'F':
				column_types_.push_back('f');
				break;
			case 'b':
			case 'B':
				column_types_.push_back('b');
				break;
			case 'e':
			case 'E':
				column_types_.push_back('i');
				break;
			default:
				break;
			}
		}
	}
}

void datatable_interpreter::_handleROWS(anh::ByteBuffer& buf)
{
	std::uint32_t count = buf.read<std::uint32_t>();
	for(std::uint32_t i=0; i < count; ++i)
	{
		DATA_ROW row;
		for(std::uint32_t j=0; j < column_types_.size(); ++j)
		{
			if(column_types_[j] == 'i')
			{
				row.columns.push_back(buf.read<std::uint32_t>());
			}
			else if(column_types_[j] == 'f')
			{
				row.columns.push_back(buf.read<float>());
			}
			else if(column_types_[j] == 's')
			{
				row.columns.push_back(buf.read<std::string>(false, true));
			}
			else if(column_types_[j] == 'b')
			{
				row.columns.push_back((buf.read<char>()) ? true : false);
			}
		}
		rows_.push_back(std::move(row));
	}
}