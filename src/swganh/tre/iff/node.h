// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <string> //for String
#include <memory> //for shared_ptr/weak_ptr
#include <cstdint> //for std::uint32_t

#include "nodetypes.h"

namespace swganh
{
namespace tre
{
	/**
		@brief The Node Base class that allows the IFFFile 
	*/
	class node
	{
	public:

		/**
			@return the name associated with this node.
		*/
		std::string& name() {return name_;}

		/**
			@return the size of this node's sub-nodes or data
		*/
		std::uint32_t size() {return size_;}
			
		/**
			@return the parent of this node.
		*/
		std::shared_ptr<node> parent() {return parent_.lock();}
			
		/**
			@brief sets the parent of this node.
			@param p the parent to set
		*/
		void parent(std::weak_ptr<node> p) {parent_ = p;}
			
		/**
			@return the node type of this node.
		*/
		virtual NODE_TYPE type() {return NODE_TYPE_BASE;}
	protected:

		/**
			@brief Constructor for a Node Object. Only callable by derived classes.
		*/
		node(std::string name, std::uint32_t size)
			: name_(name), size_(size) {}

		/**
			@brief Constructor for a Node Object. Only callable by derived classes.
		*/
		node(std::string name, std::uint32_t size, std::weak_ptr<node> parent)
			: name_(name), size_(size), parent_(parent) {}

	private:
		node(){} //Default Constructor	
				
		node(const node& n); //Copy Constructor

		std::string name_;
		std::uint32_t size_;

		std::weak_ptr<node> parent_;
	};
}
}
