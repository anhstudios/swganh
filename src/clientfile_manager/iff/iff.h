#include <stdio.h>
#include <vector>
#include <map>
#include <memory>
#include <exception>

#include "node.h"

namespace ClientFileManager
{
	namespace IFF
	{
		static class NotFoundException : public std::exception
		{
			virtual const char* what() const throw() { return "IFF Not Found.";} 
		} IFFNotFound;

		static class BadFileException : public std::exception
		{
			virtual const char* what() const throw() { return "Bad IFF File."; }
		} BadIFF;

		class IFFFile
		{
		public:
			IFFFile(std::string filename);
			IFFFile(FILE* inputstream);

			std::shared_ptr<ClientFileManager::IFF::Node> headnode(unsigned int i) {return headNodes[i];}
			unsigned int headnode_count() {return headNodes.size();}

		protected:
		private:
			void _loadIFF(FILE* inputstream);

			std::vector<std::shared_ptr<ClientFileManager::IFF::Node>> headNodes;
			
			static std::shared_ptr<ClientFileManager::IFF::Node> createNode(std::shared_ptr<std::string> name, unsigned int size);

		};
	};
};