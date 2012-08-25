// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef VISTORS_IFF_H
#define VISTORS_IFF_H

#include <memory> //For shared ptr
#include <string> //For String
#include "visitor_types.h" //For visitor types

//Forward Declarations
namespace anh
{
	class ByteBuffer;
}

namespace swganh
{
namespace tre
{
	class VisitorInterface
	{
	public:

		/**
			@brief interprets a IFF::FileNode associated with this visitor.
			This should only be called by the IFFFile code.
		*/
		virtual void visit_data(uint32_t depth, std::string name, uint32_t size, anh::ByteBuffer& data) = 0;

		/**
			@brief interprets a IFF::FolderNode associated with this visitor.
			This should only be called by the IFFFile code.
		*/
		virtual void visit_folder(uint32_t depth, std::string name, uint32_t size) = 0;
	};
}
}
#endif