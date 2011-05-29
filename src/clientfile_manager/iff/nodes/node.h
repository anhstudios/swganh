#ifndef IFF_NODE_H
#define IFF_NODE_H

#include <string>
#include <memory>
#include <iostream>
#include <cstdint>

#include "nodetypes.h"

namespace ClientFileManager
{
	namespace IFF
	{
		class Node
		{
		public:

			std::shared_ptr<std::string> name() {return name_;}
			std::uint32_t size() {return size_;}
			std::shared_ptr<Node> parent() {return parent_;}

			void parent(std::shared_ptr<Node> p) {parent_ = p;}

			virtual ClientFileManager::NODE_TYPE type() {return NODE_TYPE_BASE;}

			static bool isNodeNameCharacter(char c)
			{
				if(c >= 48 && c <= 57 || c >= 65 && c <= 90 || c == 32)
					return true;
				else
					return false;
			}

			static unsigned int getNameSize(char namedata[], unsigned int size);
			static std::shared_ptr<std::string> getIFFName(FILE* input);

		protected:
			Node(std::shared_ptr<std::string> name, std::uint32_t size)
				: name_(name), size_(size) {}

			Node(std::shared_ptr<std::string> name, std::uint32_t size, std::shared_ptr<Node> parent)
				: name_(name), size_(size), parent_(parent) {}


			std::shared_ptr<Node> parent_;

		private:
			Node();
			Node(const Node& n);

			std::shared_ptr<std::string> name_;
			std::uint32_t size_;
		};
	};
};

static void memflip(char buf[], unsigned int size)
{
	unsigned int half = size/2;
	for(unsigned int i=0; i < half; ++i) 
	{
		buf[i] ^= buf[size-i-1];
		buf[size-i-1] ^= buf[i];
		buf[i] ^= buf[size-i-1];
	}
}

template <class T> T readHost(FILE* input)
{
	char* buf = new char[sizeof(T)];
	fread(buf, sizeof(T), 1, input);

	T t = *((T*)buf);

	delete[] buf;
	return t;
}


template<class T> T readNet(FILE* input)
{
	char* buf = new char[sizeof(T)];
	fread(buf, sizeof(T), 1, input);

	memflip(buf, sizeof(T));

	T t = *((T*)buf);

	delete[] buf;
	return t;
}

template <> std::string readHost <std::string> (FILE* input);

#endif