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
			FolderNode(std::string& name, unsigned int size) : Node(name, size) 
			{

			}

			/**
				@return the node type of this node.
			*/
			virtual ClientFileManager::NODE_TYPE type() {return ClientFileManager::NODE_TYPE_FOLDER;}

			/**
				@return the number of children this node has.
			*/
			unsigned int children_count() { return children.size();}
			
			/**
				@param id which node to return
				@return the IFFNode specified by the id.
			*/
			std::shared_ptr<IFF::Node> get(unsigned int id) {children[id];}

			/**
				@brief adds a node to the list
				@param child the node to add
			*/
			void add(std::shared_ptr<IFF::Node> child) {children.push_back(child);}

			/**
				@brief removes a node from the list.
				@param id the node to remove
			*/
			void remove(unsigned int id) {children.erase(children.begin()+id);}

		private:
			std::vector<std::shared_ptr<IFF::Node>> children;
		};
	};
};

#endif