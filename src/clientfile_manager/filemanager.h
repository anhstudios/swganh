#include <string>
#include <boost/filesystem.hpp>
#include <vector>
#include <map>
#include <memory>

namespace ClientFileManager
{
	namespace IFF
	{
		class IFFFile;
		namespace Interpreters
		{
			class IFFInterpreter;
		};
	};

	namespace TRE
	{
		class TreeArchive;
		struct TreeEntry;
	};

	typedef std::map<std::string, IFF::IFFFile*> LoadedMap;
	typedef std::map<std::string, IFF::IFFFile*>::iterator LoadedMapIterator;

	typedef std::map<std::string, std::shared_ptr<TRE::TreeEntry>> FileMap;
	typedef std::map<std::string, std::shared_ptr<TRE::TreeEntry>>::iterator FileMapIterator;

	class FileManager
	{
	public:
		FileManager(std::string basedir);
		FileManager(boost::filesystem::path& basedir);

		IFF::IFFFile* getFile(const std::string& filename, IFF::Interpreters::IFFInterpreter* interpreter);
	private:
		
		void _loadSWGTREConfig();

		boost::filesystem::path basedir_;
		std::vector<TRE::TreeArchive*> trees;
		LoadedMap loadedFileMap_;
		FileMap fileMap_;
	};
};