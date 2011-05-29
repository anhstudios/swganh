#include <map>

namespace ClientFileManager
{
	namespace IFF
	{
		class IFFFile;
	};

	class SWGFileInterpreter
	{
	public:

		SWGFileInterpreter(IFF::IFFFile* iff) 
			: iff(iff)
		{
		}

		virtual std::string getName() = 0;

	protected:
		IFF::IFFFile* iff;
	private:

	};

	class SWGFileManager
	{
	public:



	protected:

	private:
		std::map<std::string, std::shared_ptr<IFF::IFFFile>> loadedFiles;
	};
};