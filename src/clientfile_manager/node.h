#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <string>

namespace ClientFileManager
{
	namespace IFF
	{
		class Node
		{
		public:
			enum NODE_TYPE
			{
				//You should never see these. They are here for completeness.
				NODE_TYPE_UNKNOWN,
				NODE_TYPE_BASE,
		
				//These are the default types. 
				//They mean that no specialized type could be found.
				NODE_TYPE_FOLDER,
				NODE_TYPE_DATA
			};

			Node(boost::shared_ptr<std::string> name, unsigned int size)
				: name(name), size(size) {}

			Node(boost::shared_ptr<std::string> name, unsigned int size, boost::shared_ptr<Node> parent)
				: name(name), size(size), parent(parent) {}

			boost::shared_ptr<std::string> name() const throw() {return name;}
			unsigned int size() const throw() {return size;}
			boost::shared_ptr<Node> parent() const throw() {return parent;}
			virtual NODE_TYPE type() const throw() {return NODE_TYPE_BASE;}

		protected:
			boost::shared_ptr<Node> parent;

		private:
			Node();
			Node(const Node& n);

			boost::shared_ptr<std::string> name;
			unsigned int size;
		};
	};
};