#ifndef IFF_H
#define IFF_H

#include <stdio.h>
#include <vector>
#include <map>
#include <cstdint>
#include <memory>
#include <functional>
#include <exception>

#include <anh/byte_buffer.h>

namespace ClientFileManager
{
	namespace IFF
	{
		class Node;
		namespace Interpreters
		{
			class IFFInterpreter;
		};

		typedef std::function<std::shared_ptr<IFF::Node>(std::shared_ptr<std::string>, std::uint32_t)> IFFCreatorCallback;
		typedef std::map<std::string, IFFCreatorCallback> NodeIndex;
		typedef std::map<std::string, IFFCreatorCallback>::iterator NodeIndexIterator;

		class NotFoundException : public std::exception
		{
		public:
			NotFoundException(std::string message)
				: message_(message)
			{
			}
			
			virtual const char* what() const throw() { return message_.c_str();} 
		private:
			std::string message_;
		};
		class BadIFFException : public std::exception
		{
		public:
			BadIFFException(std::string message)
				: message_(message)
			{
			}

			virtual const char* what() const throw() { return message_.c_str(); }
		private:
			std::string message_;
		};

		class IFFFile
		{
		public:
			IFFFile(IFFFile&& other);
			IFFFile& operator=(IFFFile&& other);
			IFFFile(std::string filename, std::shared_ptr<Interpreters::IFFInterpreter> interpret);
			IFFFile(FILE* inputstream, std::shared_ptr<Interpreters::IFFInterpreter> interpret);
			IFFFile(anh::ByteBuffer& input, std::shared_ptr<Interpreters::IFFInterpreter> interpret);

			std::shared_ptr<Interpreters::IFFInterpreter> getInterpreter() { return interpreter_;}
			std::shared_ptr<ClientFileManager::IFF::Node> headnode(unsigned int i) {return headNodes[i];}
			unsigned int headnode_count() {return headNodes.size();}

			

		protected:
		private:
			void _loadIFF(FILE* stream);
			void _loadIFF(anh::ByteBuffer& inputstream);
			std::vector<std::shared_ptr<ClientFileManager::IFF::Node>> headNodes;
			std::shared_ptr<Interpreters::IFFInterpreter> interpreter_;
		};
	};
};

#endif