#include <memory>
#include <string>

namespace anh
{
	class ByteBuffer;
}

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
				std::string readStringFromByteBuffer(anh::ByteBuffer& b);
			private:
			};
		};
	};
};