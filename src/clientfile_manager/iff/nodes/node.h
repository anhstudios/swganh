#ifndef IFF_NODE_H
#define IFF_NODE_H

#include <string> //for String
#include <memory> //for shared_ptr/weak_ptr
#include <cstdint> //for std::uint32_t

#include "nodetypes.h"

namespace ClientFileManager
{
	namespace IFF
	{
		/**
			@brief The Node Base class that allows the IFFFile 
		*/
		class Node
		{
		public:

			/**
				@return the name associated with this node.
			*/
			std::string name() {return name_;}

			/**
				@return the size of this node's sub-nodes or data
			*/
			std::uint32_t size() {return size_;}
			
			/**
				@return the parent of this node.
			*/
			std::weak_ptr<Node> parent() {return parent_;}
			
			/**
				@brief sets the parent of this node.
				@param p the parent to set
			*/
			void parent(std::weak_ptr<Node> p) {parent_ = p;}
			
			/**
				@return the node type of this node.
			*/
			virtual ClientFileManager::NODE_TYPE type() {return NODE_TYPE_BASE;}
		protected:

			/**
				@brief Constructor for a Node Object. Only callable by derived classes.
			*/
			Node(std::string name, std::uint32_t size)
				: name_(name), size_(size) {}

			/**
				@brief Constructor for a Node Object. Only callable by derived classes.
			*/
			Node(std::string name, std::uint32_t size, std::weak_ptr<Node> parent)
				: name_(name), size_(size), parent_(parent) {}
			

			std::weak_ptr<Node> parent_;
		private:
			Node(); //Default Constructor		
			Node(const Node& n); //Copy Constructor

			std::string name_;
			std::uint32_t size_;
		};
	};
};

#endif