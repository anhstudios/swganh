#include <memory>

namespace ClientFileManager
{
	namespace IFF
	{
		class FileNode;

		namespace Interpreters
		{
			class IFFInterpreter
			{
			public:
				virtual void interpret(IFF::FileNode* node) = 0;
			protected:
			private:
			};
		};
	};
};