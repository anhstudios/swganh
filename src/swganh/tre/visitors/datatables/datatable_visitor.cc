// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "datatable_visitor.h"
#include "../../iff/iff.h"
#include "../../iff/filenode.h"
#include "../../iff/foldernode.h"

#include <regex>

using namespace std;
using namespace swganh::tre;

void DatatableVisitor::visit_folder(uint32_t depth, std::shared_ptr<folder_node> node)
{
}

void DatatableVisitor::visit_data(uint32_t depth, std::shared_ptr<file_node> node)
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

void DatatableVisitor::_handle0001COLS(anh::ByteBuffer& buf)
{
	std::uint32_t count = buf.read<std::uint32_t>();
	for(unsigned int i=0; i < count; ++i)
	{
		column_names_.push_back(buf.read<std::string>(false, true));
	}
}

void DatatableVisitor::_handleTYPE(anh::ByteBuffer& buf)
{
	std::string buffer;
	for(unsigned int i=0; i < column_names_.size(); ++i)
	{
		buffer = buf.read<std::string>(false, true);
		if(buffer.size() > 0)
		{
			switch(buffer[0])
			{
			case 'h':
			case 'i':
			case 'I':
			case 'e':
			case 'E':
			case 'b':
			case 'B':
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
			
				break;
			default:
				break;
			}
		}
	}
}

void DatatableVisitor::_handleROWS(anh::ByteBuffer& buf)
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