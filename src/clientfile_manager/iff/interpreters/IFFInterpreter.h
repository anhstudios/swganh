#ifndef INTERPRETERS_IFF_H
#define INTERPRETERS_IFF_H

#include <map>
#include <memory>
#include <string>
#include <functional>
#include <boost/flyweight.hpp>
#include "InterpreterTypes.h"


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
			typedef std::function<void(anh::ByteBuffer&)> NodeFunctor;
			typedef std::map<std::string, NodeFunctor> NodeNameIndex;
			typedef std::map<std::string, NodeFunctor>::const_iterator NodeNameIndexIterator;

			class IFFInterpreter
			{
			public:

				IFFInterpreter()
				{
				}

				virtual InterpreterType getType() { return IFF_INTERPRETER; }

				virtual void interpret(std::shared_ptr<IFF::FileNode> node) = 0;
			protected:
				std::string readStringFromByteBuffer(anh::ByteBuffer& b);
				NodeNameIndex nameLookup_;
			};
		};
	};
};

#endif