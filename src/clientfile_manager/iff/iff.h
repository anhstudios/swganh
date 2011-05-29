#include <stdio.h>
#include <vector>
#include <map>
#include <cstdint>
#include <memory>
#include <functional>
#include <exception>

namespace ClientFileManager
{
	namespace IFF
	{
		class Node;

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
			IFFFile(std::string filename);
			IFFFile(FILE* inputstream);

			std::shared_ptr<ClientFileManager::IFF::Node> headnode(unsigned int i) {return headNodes[i];}
			unsigned int headnode_count() {return headNodes.size();}
		private:
			void _loadIFF(FILE* inputstream);

			std::vector<std::shared_ptr<ClientFileManager::IFF::Node>> headNodes;
			
			static std::shared_ptr<ClientFileManager::IFF::Node> createNode(std::shared_ptr<std::string> name, unsigned int size);

			static void IFFFile::prepIndex();
			static NodeIndex index;
		};
	};
};