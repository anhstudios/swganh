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
		virtual void visit_data(uint32_t depth, std::string name, uint32_t size, anh::ByteBuffer& data);

		/**
			@brief interprets a IFF::FolderNode associated with this interpreter.
			This should only be called by the IFFFile code.
		*/
		virtual void visit_folder(uint32_t depth, std::string name, uint32_t size);

		struct slot_entry
		{
			std::string name;
			bool global;
			bool canMod;
			bool exclusive;
			std::string hardpoint_name;
			std::uint32_t unkValue;
		};

		size_t count() {return slots_.size();}
		size_t findSlotByName(const std::string& name);
		slot_entry& entry(size_t id) { return slots_[id];}

	private:
		void _handle0006DATA(anh::ByteBuffer& buf);

		std::vector<slot_entry> slots_;
	};
}
}
#endif