/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2011 The SWG:ANH Team
---------------------------------------------------------------------------------------
Use of this source code is governed by the GPL v3 license that can be found
in the COPYING file or at http://www.gnu.org/licenses/gpl-3.0.html

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,a
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
---------------------------------------------------------------------------------------
*/

#ifndef IFF_NODE_H
#define IFF_NODE_H

#include <string> //for String
#include <memory> //for shared_ptr/weak_ptr
#include <cstdint> //for std::uint32_t

#include <anh/clientfile_manager/iff/nodetypes.h>

namespace anh
{
	namespace clientfile_manager
	{
		namespace iff
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
		};
	};
};
#endif