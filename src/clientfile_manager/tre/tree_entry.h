#include <cstdint>

namespace ClientFileManager
{
	namespace TRE
	{
		class TreeArchive;
		struct TreeEntry
		{
			std::uint32_t checksum;
			std::uint32_t uncompressed_size;
			std::uint32_t fileOffset;
			std::uint32_t compressionType;
			std::uint32_t compressed_size;
			std::uint32_t name_offset;

			std::string nameString;
			TreeArchive* parentTree;
		};
	};
};