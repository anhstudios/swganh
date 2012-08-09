// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SLOT_DEFINITION_VISITOR_H
#define SLOT_DEFINITION_VISITOR_H

#include "../visitor_interface.h"

#include <cstdint>
#include <vector>

namespace swganh
{
namespace tre
{
	class SlotDefinitionVisitor : public VisitorInterface
	{
	public:
		/**
			@brief returns the InterpreterType associated with this Interpreter
		*/
		virtual VisitorType getType() { return SLOT_DEFINITION_VISITOR; }

		/**
			@brief interprets a IFF::FileNode associated with this interpreter.
			This should only be called by the IFFFile code.
		*/
		virtual void visit_data(std::shared_ptr<file_node> node);

		/**
			@brief interprets a IFF::FolderNode associated with this interpreter.
			This should only be called by the IFFFile code.
		*/
		virtual void visit_folder(std::shared_ptr<folder_node> node);

		struct slot_entry
		{
			std::string name;
			char unkFlag1;
			std::string hardpoint_name;
			std::uint32_t unkValue;
		};

		size_t count() {return slots_.size();}
		slot_entry& entry(size_t id) { return slots_[id];}

	private:
		void _handle0006DATA(anh::ByteBuffer& buf);

		std::vector<slot_entry> slots_;
	};
}
}
#endif