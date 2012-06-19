// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "datatable_reader.h"

#include <cstring>
#include <algorithm>
#include <stdexcept>
#include <iterator>

#include <boost/lexical_cast.hpp>

#include "anh/utilities.h"

using namespace swganh::tre::readers;

using anh::bigToHost;
using boost::any_cast;
using boost::lexical_cast;
using std::for_each;
using std::move;
using std::out_of_range;
using std::runtime_error;
using std::string;
using std::transform;
using std::unordered_map;
using std::vector;

struct IffHeader
{
    char form[4];
    uint32_t size;
    char type[4];
};

DatatableReader::DatatableReader(vector<char> input)
    : current_row_(-1)
    , input_(move(input))
{
    ValidateFile(input_);

    uint32_t data_position = sizeof(IffHeader) * 2; // Starts after the second iff header
    column_header_ = reinterpret_cast<const ColumnHeader*>(&input_[data_position]);
    column_offset_ = &input_[data_position + sizeof(ColumnHeader)];

    ParseColumnNames();

    data_position += sizeof(ColumnHeader) - sizeof(uint32_t) + bigToHost(column_header_->size);
    type_header_ = reinterpret_cast<const TypeHeader*>(&input_[data_position]);
    type_offset_ =  &input_[data_position + sizeof(TypeHeader)];

    ParseColumnTypes();

    data_position += sizeof(TypeHeader) + bigToHost(type_header_->size);
    row_header_ = reinterpret_cast<const RowHeader*>(&input_[data_position]);
    row_offset_ = &input_[data_position + sizeof(RowHeader)];

    ParseRows();
}

uint32_t DatatableReader::CountRows() const
{
    return row_header_->count;
}

uint32_t DatatableReader::CountColumns() const
{
    return column_names_.size();
}

const vector<string>& DatatableReader::GetColumnNames() const
{
    return column_names_;
}

ColumnsMetaData DatatableReader::GetColumnsMetaData() const
{
    if (column_names_.size() != column_types_.size())
    {
        throw runtime_error("Datatable header information is corrupt");
    }

    ColumnsMetaData meta_data;

    transform(
        begin(column_names_), end(column_names_),
        begin(column_types_), inserter(meta_data, end(meta_data)),
        [] (const std::string& name, const std::string& type) -> ColumnMetaData
    {
        ColumnMetaData meta_data;
        meta_data.name = name;
        meta_data.type = type;

        return meta_data;
    });

    return meta_data;
}

bool DatatableReader::Next()
{
    ++current_row_;

    return static_cast<uint32_t>(current_row_) < row_header_->count;
}

DatatableRow DatatableReader::GetRow()
{
    if (static_cast<uint32_t>(current_row_) >= row_header_->count)
    {
        throw out_of_range("Accessed past the end of the rows");
    }

    DatatableRow row;

    transform(
        begin(column_names_), end(column_names_),
        row_data_[current_row_].begin(), inserter(row, row.end()),
        [] (string name, DatatableCell& value)
    {
        return make_pair(name, &value);
    });

    return row;
}

void DatatableReader::ValidateFile(const vector<char>& input) const
{
    const IffHeader* header = reinterpret_cast<const IffHeader*>(&input[0]);

    if (string(header->type, 4).compare("DTII") != 0)
    {
        throw runtime_error("Invalid datatable file format");
    }
}

void DatatableReader::ParseColumnNames()
{
    string tmp;
    for (uint32_t i = 0, offset = 0; i < column_header_->count; ++i)
    {
        tmp = string(reinterpret_cast<const char*>(column_offset_ + offset));
        column_names_.push_back(tmp);
        offset += tmp.size() + 1;
    }
}

void DatatableReader::ParseColumnTypes()
{
    string tmp;
    for (uint32_t i = 0, offset = 0; i < column_header_->count; ++i)
    {
        tmp = string(reinterpret_cast<const char*>(type_offset_ + offset));
        column_types_.push_back(tmp);
        offset += tmp.size() + 1;
    }
}

void DatatableReader::ParseRows()
{
    for (uint32_t i = 0, offset = 0; i < row_header_->count; ++i)
    {
        vector<DatatableCell> row;

        for_each(
            begin(column_types_),
            end(column_types_),
            [this, &row, &offset] (const string& type)
        {
            offset += ParseColumn(static_cast<uint8_t>(type[0]), offset, row);
        });

        row_data_.push_back(move(row));
    }
}


uint32_t DatatableReader::ParseColumn(uint8_t type, uint32_t offset, vector<DatatableCell>& row)
{
    uint32_t size = 0;
    DatatableCell cell;

    switch(type)
    {
    case 'f':
        cell.SetValue(reinterpret_cast<const float*>(row_offset_ + offset));

        size = 4;
        break;

    case 'b':
    case 'h':
    case 'e':
    case 'z':
    case 'i':
    case 'I':
        cell.SetValue(reinterpret_cast<const int*>(row_offset_ + offset));

        size = 4;
        break;

    case 's':
        cell.SetValue(reinterpret_cast<const char*>(row_offset_ + offset));

        string tmp(reinterpret_cast<const char*>(row_offset_ + offset));
        size = tmp.length() + 1;
        break;
    }

    row.push_back(move(cell));

    return size;
}

string DatatableCell::ToString() const
{
    string str;

    try
    {
        if (value_.type() == typeid(const int*))
        {
            str = lexical_cast<string>(*any_cast<const int*>(value_));
        }
        else if (value_.type() == typeid(const bool*))
        {
            str = lexical_cast<string>(*any_cast<const bool*>(value_));
        }
        else if (value_.type() == typeid(const float*))
        {
            str = lexical_cast<string>(*any_cast<const float*>(value_));
        }
        else if (value_.type() == typeid(const char*))
        {
            str = any_cast<const char*>(value_);
        }
        else
        {
            str = "";
        }
    }
    catch (...)
    {
        str = "";
    }

    return str;
}
