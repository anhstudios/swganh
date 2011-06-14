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

#ifndef INTERPRETERS_POB_H
#define INTERPRETERS_POB_H

#include "../interpreter_interface.h"
#include <vector>
#include <map>
#include <functional>
#include <cstdint>
#include <glm/glm.hpp>

namespace anh
{
	class ByteBuffer;

	namespace clientfile_manager
	{
		namespace interpreters
		{
			class pob_interpreter : public interpreter_interface
			{
			public:
				typedef std::function<void(pob_interpreter*, anh::ByteBuffer& buffer)> NodeFunctor;
				typedef std::map<std::string, NodeFunctor> NodeNameIndex;
				typedef std::map<std::string, NodeFunctor>::iterator NodeNameIndexIterator;

				/**
					@brief Constructor for a pob interpreter
				*/
				pob_interpreter();

				/**
					@brief returns the InterpreterType associated with this Interpreter
				*/
				virtual InterpreterType getType() { return POB_INTERPRETER; }

				/**
					@brief interprets a IFF::FileNode associated with this interpreter.
					This should only be called by the IFFFile code.
				*/
				virtual void interpret_data(std::shared_ptr<iff::file_node> node);

				/**
					@brief interprets a IFF::FolderNode associated with this interpreter.
					This should only be called by the IFFFile code.
				*/
				virtual void interpret_folder(std::shared_ptr<iff::folder_node> node);

				/**
					@brief A portal object used inside the POB files.
				*/
				class Portal
				{
				public:
					std::vector<glm::vec3> vertices;
				};

				/**
					@brief A Link between a Cell and a Portal Object
				*/
				class Link
				{
				public:
					std::uint32_t portal_id; //portal id
					
					char unkFlag2;
					std::uint32_t dst_cellid; //destination cell id

					//BELOW HERE IS LIKELY DOOR STUFF WE ARE NOT CONCERNED WITH
					std::string doorname;

					glm::vec3 unk1; //Seems like an inefficient bitmask
					float unk2; //scale?

					glm::vec3 unk3; //Seems like an inefficient bitmask
					float unk4; //Scale?

					glm::vec3 unk5; //Seems like an inefficient bitmask
					float unk6; //Scale?

				};

				/**
					@A simple triangle class that references different vertices.
				*/
				class triangle
				{
				public:
					std::uint32_t a;
					std::uint32_t b;
					std::uint32_t c;
				};

				/**
					@brief A cell class that has a certain shape, id, mesh, and collision
				*/
				class Cell
				{
				public:
					//0005DATA
					char unkFlag2;
					std::string name;
					char unkFlag3;
					std::string mesh;
					char unkFlag4;
					std::string collision;

					//0000VERT
					std::vector<glm::vec3> vertices;

					//INDX
					std::vector<triangle> triangles;

					//PRTL0004
					std::vector<Link> links;
				};

				/**
					@return the number of cells in this pob file
				*/
				unsigned int cell_count() { return cells_.size(); }
				
				/**
					@return the number of portals in this pob file.
				*/
				unsigned int portal_count() { return portals_.size(); }

				/**
					@brief Fetches a portal object
					@param id the id of the portal to fetch
					@return the fetched portal
				*/
				const Portal& getPortal(unsigned int id) { return portals_[id]; }
				
				/**
					@brief Fetches a cell object
					@param id the id of the cell to fetch
					@return the fetched cell
				*/
				const Cell& getCell(unsigned int id) { return cells_[id]; }

				/**
					@brief a simple output function for debugging values.
				*/
				void debug();

			private:

				//Internal Node Handling functions
				static void _handle0003DATA(pob_interpreter*, anh::ByteBuffer& data);
				static void _handlePRTL(pob_interpreter*, anh::ByteBuffer& data);
				static void _handle0005DATA(pob_interpreter*, anh::ByteBuffer& data);
				static void _handle0000VERT(pob_interpreter*, anh::ByteBuffer& data);
				static void _handleINDX(pob_interpreter*, anh::ByteBuffer& data);
				static void _handlePRTL0004(pob_interpreter*, anh::ByteBuffer& data);

				//Internal Containers for portals and cells
				std::vector<Portal> portals_;
				std::vector<Cell> cells_;

				static NodeNameIndex nameLookup_;
			};
		};
	};

};
#endif