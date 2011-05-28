#ifndef IFF_NODE_H
#define IFF_NODE_H

#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <string>
#include <iostream>

#include "nodetypes.h"

namespace ClientFileManager
{
	namespace IFF
	{
		class Node
		{
		public:

			boost::shared_ptr<std::string> name() {return name_;}
			unsigned int size() {return size_;}
			boost::shared_ptr<Node> parent() {return parent_;}
			virtual ClientFileManager::NODE_TYPE type() {return NODE_TYPE_BASE;}

			static bool isNodeNameCharacter(char c)
			{
				if(c >= 48 && c <= 57 || c >= 65 && c <= 90 || c == 32)
					return true;
				else
					return false;
			}

			static unsigned int getNameSize(char namedata[], unsigned int size);
			static boost::shared_ptr<std::string> getIFFName(std::istream& input);

		protected:
			Node(std::istream& input);

			Node(boost::shared_ptr<std::string> name, unsigned int size)
				: name_(name), size_(size) {}

			Node(boost::shared_ptr<std::string> name, unsigned int size, boost::shared_ptr<Node> parent)
				: name_(name), size_(size), parent_(parent) {}


			boost::shared_ptr<Node> parent_;

		private:
			Node();
			Node(const Node& n);

			boost::shared_ptr<std::string> name_;
			unsigned int size_;
		};
	};
};

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

template <class T> T readHost(std::istream& input)
{
	T t;
	input >> t;
	return t;
}

template<class T> T readNet(std::istream& input)
{
	T t;
	input >> t;
	memflip((char*)&t, sizeof(T));
	return t;
}

#endif