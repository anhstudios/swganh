#include "../interpreter_interface.h"

#include <cstdint>
#include <vector>

namespace anh
{
	namespace clientfile_manager
	{
		namespace interpreters
		{
			class slot_definition_interpreter : public interpreter_interface
			{
			public:
				/**
					@brief returns the InterpreterType associated with this Interpreter
				*/
				virtual InterpreterType getType() { return SLOT_DEFINITION_INTERPRETER; }

				/**
					@brief interprets a IFF::FileNode associated with this interpreter.
					This should only be called by the IFFFile code.
				*/
				virtual void interpret(std::shared_ptr<iff::file_node> node);

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
		};
	};
};