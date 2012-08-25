// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "node.h"
#include <vector>

namespace swganh
{
namespace tre
{
	class folder_node : public node
	{
	public:
		folder_node(std::string& name, unsigned int size) : node(name, size) 
		{

		}

		folder_node(std::string& name, unsigned int size, std::shared_ptr<node> parent) : node(name, size, parent) 
		{

		}

		/**
			@return the node type of this node.
		*/
		virtual NODE_TYPE type() {return NODE_TYPE_FOLDER;}

		/**
			@return the number of children this node has.
		*/
		unsigned int children_count() { return children.size();}
			
		/**
			@param id which node to return
			@return the IFFNode specified by the id.
		*/
		std::shared_ptr<node> child(unsigned int id) {return children[id];}

		/**
			@brief adds a node to the list
			@param child the node to add
		*/
		void add(std::shared_ptr<node> child) {children.push_back(child);}

		/**
			@brief removes a node from the list.
			@param id the node to remove
		*/
		void remove(unsigned int id) {children.erase(children.begin()+id);}

	private:
		std::vector<std::shared_ptr<node>> children;
	};
}
}
