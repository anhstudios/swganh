// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "../visitor_interface.h"

#include <cstdint>
#include <vector>

namespace swganh
{
namespace tre
{
	class SlotDescriptorVisitor : public VisitorInterface
	{
	public:
		/**
			@brief returns the InterpreterType associated with this Interpreter
		*/
		virtual VisitorType getType() { return SLOT_DESCRIPTOR_VISITOR; }

		/**
			@brief interprets a IFF::FileNode associated with this interpreter.
			This should only be called by the IFFFile code.
		*/
		virtual void visit_data(uint32_t depth, std::shared_ptr<file_node> node);

		/**
			@brief interprets a IFF::FolderNode associated with this interpreter.
			This should only be called by the IFFFile code.
		*/
		virtual void visit_folder(uint32_t depth, std::shared_ptr<folder_node> node);

		size_t available_count() {return slots_available.size();}
		std::string& slot(size_t id) {return slots_available[id];}

	private:
		void _handle0000DATA(anh::ByteBuffer& buf);

		std::vector<std::string> slots_available;
	};
}
}
