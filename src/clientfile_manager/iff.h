#include <boost/shared_ptr.hpp>
#include <iostream>
#include <vector>
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
			IFFFile(std::string filename) throw (NotFoundException, BadFileException);
			IFFFile(std::istream& inputstream) throw (NotFoundException);

			boost::shared_ptr<ClientFileManager::IFF::Node> headnode(unsigned int i) {return headNodes[i];}
			unsigned int headnode_count() {return headNodes.size();}

		protected:
		private:
			void _loadIFF(std::istream& inputstream) throw (BadFileException);
		
			std::vector<boost::shared_ptr<ClientFileManager::IFF::Node>> headNodes;
		};
	};
};