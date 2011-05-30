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

			std::string& name() {return name_;}
			std::uint32_t size() {return size_;}
			Node* parent() {return parent_;}
			void parent(Node* p) {parent_ = p;}
			virtual ClientFileManager::NODE_TYPE type() {return NODE_TYPE_BASE;}

		protected:
			Node(std::string& name, std::uint32_t size)
				: name_(name), size_(size) {}

			Node(std::string& name, std::uint32_t size, Node* parent)
				: name_(name), size_(size), parent_(parent) {}


			Node* parent_;

		private:
			Node();
			Node(const Node& n);

			std::string name_;
			std::uint32_t size_;
		};
	};
};

#endif