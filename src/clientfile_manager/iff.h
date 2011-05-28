#include <boost/shared_ptr.hpp>
#include <iostream>
#include <vector>
#include <map>
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
			IFFFile(std::istream& inputstream);

			boost::shared_ptr<ClientFileManager::IFF::Node> headnode(unsigned int i) {return headNodes[i];}
			unsigned int headnode_count() {return headNodes.size();}

		protected:
		private:
			void _loadIFF(std::istream& inputstream);

			static void memflip(char buf[], unsigned int size)
			{
				unsigned int half = size/2;
				for(unsigned int i=0; i < half; ++i) 
				{
					buf[i] ^= buf[size-i];
					buf[size-i] ^= buf[i];
					buf[i] ^= buf[size-i];
				}
			}

			std::vector<boost::shared_ptr<ClientFileManager::IFF::Node>> headNodes;
			
			static boost::shared_ptr<ClientFileManager::IFF::Node> createNode(boost::shared_ptr<std::string> name, unsigned int size);

		};
	};
};