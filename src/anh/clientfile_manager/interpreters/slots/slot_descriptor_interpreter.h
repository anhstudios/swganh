#include "../interpreter_interface.h"

#include <cstdint>
#include <vector>

namespace anh
{
	namespace clientfile_manager
	{
		namespace interpreters
		{
			class slot_descriptor_interpreter : public interpreter_interface
			{
			public:
				/**
					@brief returns the InterpreterType associated with this Interpreter
				*/
				virtual InterpreterType getType() { return SLOT_DESCRIPTOR_INTERPRETER; }

				/**
					@brief interprets a IFF::FileNode associated with this interpreter.
					This should only be called by the IFFFile code.
				*/
				virtual void interpret(std::shared_ptr<iff::file_node> node);

			private:
				void _handle0000DATA(anh::ByteBuffer& buf);

				std::vector<std::string> slots_available;
			};
		};
	};
};