#ifndef IFF_FOLDERNODE_H
#define IFF_FOLDERNODE_H

#include "nodes/node.h"
#include <vector>

namespace ClientFileManager
{
	namespace IFF
	{
		class FolderNode : public Node
		{
		public:
			FolderNode(std::string& name, unsigned int size) : Node(boost::flyweight<std::string>(name), size) 
			{

			}

			virtual ClientFileManager::NODE_TYPE type() {return ClientFileManager::NODE_TYPE_FOLDER;}

			unsigned int children_count() { return children.size();}
			std::shared_ptr<IFF::Node> get(unsigned int id) {children[id];}

			void add(std::shared_ptr<IFF::Node> child) {children.push_back(child);}
			void remove(unsigned int id) {children.erase(children.begin()+id);}

		private:
			std::vector<std::shared_ptr<IFF::Node>> children;
		};
	};
};

#endif