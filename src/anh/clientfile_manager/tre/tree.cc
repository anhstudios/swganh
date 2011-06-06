/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2010 The SWG:ANH Team */

#include "tree.h"
#include "tree_entry.h"

#include <cstdint>
#include <stdio.h>
#include <zlib.h>
#include <anh/byte_buffer.h>

using namespace anh::clientfile_manager::tree;
using namespace boost::filesystem2;

anh::ByteBuffer tree_archive::getFile(const tree_entry& entry) const
{
	unsigned char* compressed_data = new unsigned char[entry.compressed_size_];

	fseek(intrl_stream, entry.fileOffset_, SEEK_SET);
	fread(compressed_data, entry.compressed_size_, 1, intrl_stream);

	unsigned char* data = new unsigned char[entry.uncompressed_size_];

	int z_result = uncompress(data, (unsigned long*)&entry.uncompressed_size_, 
								compressed_data, entry.compressed_size_);
	delete[] compressed_data;

	anh::ByteBuffer result;
	result.write(data, entry.uncompressed_size_);
	delete[] data;
	
	return result;
}

void tree_archive::loadTree_(FILE* inputstream, std::map<std::string, const tree_entry>& entries)
{
	//This is how .tre files were meant to be loaded.
	//No Unnecessary Objects.
	//No Bullshit.

	std::uint32_t record_count;
	std::uint32_t lookupData_offset;
	std::uint32_t lookupData_compression;
	std::uint32_t lookupData_currentSize;
	std::uint32_t lookupData_finalSize; //CALCULATED NOT READ
	std::uint32_t names_offset; //CALCUATED NOT READ
	std::uint32_t names_compression;
	std::uint32_t names_currentSize;
	std::uint32_t names_finalSize;


	char name[8];
	fread(name, 8, 1, inputstream);

	if(memcmp(name, "EERT5000", 8) == 0) 
	{
		/////////////////////////   HEADER   ////////////////////////////////
		fread(&record_count, 4, 1, inputstream);
		fread(&lookupData_offset, 4, 1, inputstream);
		fread(&lookupData_compression, 4, 1, inputstream);
		fread(&lookupData_currentSize, 4, 1, inputstream);
		lookupData_finalSize = record_count * 18;
		names_offset = lookupData_offset + lookupData_currentSize;
		fread(&names_compression, 4, 1, inputstream);
		fread(&names_currentSize, 4, 1, inputstream);
		fread(&names_finalSize, 4, 1, inputstream);
		
		int z_result;

		/////////////////////////DATA BLOCK////////////////////////////////
		fseek(inputstream, lookupData_offset, SEEK_SET);
		unsigned char* lookupData_block = new unsigned  char[lookupData_finalSize];
		if(lookupData_compression == 2)
		{
			unsigned char* compressed_lookupData_block = new unsigned char[lookupData_currentSize];
			fread(compressed_lookupData_block, lookupData_currentSize, 1, inputstream);
			z_result = uncompress(lookupData_block, (unsigned long*)&lookupData_finalSize, 
									compressed_lookupData_block, lookupData_currentSize);
			delete[] compressed_lookupData_block;
		}
		else
		{
			fread(lookupData_block, lookupData_finalSize, 1, inputstream);
		}
		
		/////////////////////////NAMES BLOCK////////////////////////////////
		fseek(inputstream, names_offset, SEEK_SET);
		unsigned char* names_block = new unsigned char[names_finalSize];
		if(names_compression == 2)
		{
			unsigned char* compressed_names_block = new unsigned char[names_currentSize];
			fread(compressed_names_block, names_currentSize, 1, inputstream);
			z_result = uncompress(names_block, (unsigned long*)&names_finalSize, 
									compressed_names_block, names_currentSize);
			delete[] compressed_names_block;
		}
		else
		{
			fread(names_block, names_finalSize, 1, inputstream);
		}

		unsigned int entry_count = lookupData_finalSize/24;
		for(unsigned int i=0; i < entry_count; ++i)
		{
			tree_entry latestEntry;
			memcpy(&latestEntry, lookupData_block + 24*i , 24);
			latestEntry.parentTree_ = this;
			entries.insert(std::make_pair<std::string, tree_entry>(
							std::string((char*)&names_block[latestEntry.name_offset_]), 
							std::move(latestEntry)));
		}
		
		delete[] lookupData_block;
		delete[] names_block;
	}
}