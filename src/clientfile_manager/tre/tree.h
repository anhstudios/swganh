#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <memory>

namespace anh
{
	class ByteBuffer;
};

namespace ClientFileManager
{
	namespace TRE
	{
		struct TreeEntry;

		class TreeArchive
		{
		public:
			TreeArchive(boost::filesystem::path& path, std::vector<std::shared_ptr<TreeEntry>>& entries)
				: filename_(path.file_string())
			{
				FILE* stream = fopen(path.file_string().c_str(), "rb");
				_loadTree(stream, entries);
				fclose(stream);
			}

			TreeArchive(std::string& filename, std::vector<std::shared_ptr<TreeEntry>>& entries)
				: filename_(filename)
			{	
				FILE* stream = fopen(filename.c_str(), "rb");
				_loadTree(stream, entries);
				fclose(stream);
			}

			anh::ByteBuffer* getFile(TreeEntry* entry);

		private:
			std::uint32_t record_count;
			std::uint32_t lookupData_offset;
			std::uint32_t lookupData_compression;
			std::uint32_t lookupData_currentSize;
			std::uint32_t lookupData_finalSize; //CALCULATED NOT READ
			std::uint32_t names_offset; //CALCUATED NOT READ
			std::uint32_t names_compression;
			std::uint32_t names_currentSize;
			std::uint32_t names_finalSize;

			void _loadTree(FILE* inputstream, std::vector<std::shared_ptr<TreeEntry>>& entries);
			std::string filename_;
		};
	};
};
