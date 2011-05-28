#include "node.h"
#include <vector>

namespace ClientFileManager
{
	namespace IFF
	{
		class FolderNode : public Node
		{
		public:
			FolderNode(boost::shared_ptr<std::string> name, unsigned int size) : Node(name, size) 
			{

			}

			virtual ClientFileManager::NODE_TYPE type() {return ClientFileManager::NODE_TYPE_FOLDER;}

			unsigned int children_count() { return children.size();}
			boost::shared_ptr<IFF::Node> get(unsigned int id) {children[id];}

			void add(boost::shared_ptr<IFF::Node> child) {children.push_back(child);}
			void remove(unsigned int id) {children.erase(children.begin()+id);}

		private:
			std::vector<boost::shared_ptr<IFF::Node>> children;
		};
	};
};