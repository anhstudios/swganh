// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SLOT_ARRANGEMENT_VISITOR_H
#define SLOT_ARRANGEMENT_VISITOR_H

#include "../visitor_interface.h"

#include <vector>
#include <list>

namespace swganh
{
namespace tre
{
	class SlotArrangementVisitor : public VisitorInterface
	{
	public:
		/**
			@brief returns the InterpreterType associated with this Interpreter
		*/
		virtual VisitorType getType() { return SLOT_ARRANGEMENT_VISITOR; }

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

		//const std::vector<std::string>& slots_occupied(size_t id) { return combinations_[id]; };

		std::list<std::vector<std::string>>::const_iterator begin() {return combinations_.cbegin();}
		std::list<std::vector<std::string>>::const_iterator end() {return combinations_.cend();}

	private:
		void _handle0000ARG(anh::ByteBuffer& buf);

		std::list<std::vector<std::string>> combinations_;

	};
}
}

#endif