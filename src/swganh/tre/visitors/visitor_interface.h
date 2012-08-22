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
	class file_node;
	class folder_node;

	class VisitorInterface
	{
	public:

		/**
			@brief returns the VisitorType associated with this visitor
		*/
		virtual VisitorType getType() { return IFF_VISITOR; }

		/**
			@brief interprets a IFF::FileNode associated with this visitor.
			This should only be called by the IFFFile code.
		*/
		virtual void visit_data(uint32_t depth, std::shared_ptr<file_node> node) = 0;

		/**
			@brief interprets a IFF::FolderNode associated with this visitor.
			This should only be called by the IFFFile code.
		*/
		virtual void visit_folder(uint32_t depth, std::shared_ptr<folder_node> node) = 0;
	};
}
}
#endif