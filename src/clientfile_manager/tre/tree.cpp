#include <cstdint>
#include <stdio.h>

#include "tree.h"
#include "tree_entry.h"
#include <zlib.h>
#include <anh/byte_buffer.h>

using namespace ClientFileManager::TRE;

anh::ByteBuffer TreeArchive::getFile(TreeEntry& entry)
{
	unsigned char* compressed_data = new unsigned char[entry.compressed_size];

	FILE* filestream = fopen(filename_.c_str(), "rb");
	fseek(filestream, entry.fileOffset, SEEK_SET);
	fread(compressed_data, entry.compressed_size, 1, filestream);
	fclose(filestream);

	unsigned char* data = new unsigned char[entry.uncompressed_size];

	int z_result = uncompress(data, (unsigned long*)&entry.uncompressed_size, 
								compressed_data, entry.compressed_size);
	delete[] compressed_data;

	anh::ByteBuffer result;
	result.write(data, entry.uncompressed_size);
	delete[] data;
	
	return result;
}

void TreeArchive::_loadTree(FILE* inputstream, std::vector<TreeEntry>& entries)
{
	//This is how .tre files were meant to be loaded.
	//No Unnecessary Objects.
	//No Bullshit.

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
			TreeEntry latestEntry;
			memcpy(&latestEntry, lookupData_block + 24*i , 24);
			latestEntry.parentTree = this;
			latestEntry.nameString = std::string((char*)&names_block[latestEntry.name_offset]);
			entries.push_back(std::move(latestEntry));
		}
		
		delete[] lookupData_block;
		delete[] names_block;
	}


}