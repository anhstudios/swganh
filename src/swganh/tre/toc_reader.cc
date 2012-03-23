// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "toc_reader.h"

using namespace swganh::tre;

using std::ifstream;
using std::ios_base;
using std::string;

TocReader::TocReader(const string& filename)
: filename_(filename)
{
    input_stream_.exceptions(ifstream::failbit | ifstream::badbit);
    input_stream_.open(filename_.c_str(), ios_base::binary);

    ReadHeader();
}

void TocReader::ReadHeader()
{    
    input_stream_.read(reinterpret_cast<char*>(&header_), sizeof(header_));
}


const TocHeader& TocReader::GetHeader() const
{
    return header_;
}
