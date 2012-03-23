// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_TRE_TOC_READER_H_
#define SWGANH_TRE_TOC_READER_H_

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

namespace swganh {
namespace tre {

#pragma pack(1)

	struct TocHeader
	{
		char file_type[4];
		char file_version[4];
		uint32_t format;
		uint32_t record_count;
        uint32_t info_compressed_size;
		uint32_t info_uncompressed_size;
		uint32_t size;
		uint32_t tre_count;
		uint32_t tre_name_size;
	};

    struct TocResourceInfo
    {
        uint16_t tre_id;
        uint32_t checksum;
        uint32_t name_length;
        uint32_t data_offset;
        uint32_t data_compressed_size;
        uint32_t data_uncompressed_size;
    };

#pragma pack()

	class TocReader
	{
	public:
		explicit TocReader(const std::string& filename);

		const TocHeader& GetHeader() const;

	private:
        void ReadHeader();

        std::vector<char> tre_name_block_;
        std::vector<char> file_name_block_;
        std::vector<TocResourceInfo> info_block_;
        
        std::ifstream input_stream_;
		TocHeader header_;
        std::string filename_;
	};

}}  // namespace swganh::tre

#endif  // SWGANH_TRE_TOC_READER_H_
