#include "../interpreter_interface.h"

#include <vector>
#include <list>

namespace anh
{
	namespace clientfile_manager
	{
		namespace interpreters
		{
			class slot_arrangement_interpreter : public interpreter_interface
			{
			public:
				/**
					@brief returns the InterpreterType associated with this Interpreter
				*/
				virtual InterpreterType getType() { return SLOT_ARRANGEMENT_INTERPRETER; }

				/**
					@brief interprets a IFF::FileNode associated with this interpreter.
					This should only be called by the IFFFile code.
				*/
				virtual void interpret(std::shared_ptr<iff::file_node> node);

				//const std::vector<std::string>& slots_occupied(size_t id) { return combinations_[id]; };

			private:
				void _handle0000ARG(anh::ByteBuffer& buf);

				//We might want this to be a vector, but I'm not sure.
				std::list<std::vector<std::string>> combinations_;

			};
		};
	};
};