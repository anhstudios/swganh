#include "../interpreter_interface.h"

#include <map>
#include <string>
#include <list>
#include <vector>
#include <cstdint>
#include <boost/any.hpp>

namespace anh
{
	namespace clientfile_manager
	{
		namespace interpreters
		{
			class datatable_interpreter : public interpreter_interface
			{
			public:
				/**
					@brief returns the InterpreterType associated with this Interpreter
				*/
				virtual InterpreterType getType() { return DATATABLE_INTERPRETER; }

				/**
					@brief interprets a IFF::FileNode associated with this interpreter.
					This should only be called by the IFFFile code.
				*/
				virtual void interpret(std::shared_ptr<iff::file_node> node);

				struct DATA_ROW
				{
					std::vector<boost::any> columns;
				};

				const std::vector<std::string>& column_names() { return column_names_; }
				const std::vector<char>& column_types() { return column_types_; }
					
				std::list<DATA_ROW>::const_iterator begin_itr() { return rows_.cbegin(); }
				std::list<DATA_ROW>::const_iterator end_itr() { return rows_.cend(); }

			private:
				void _handle0001COLS(anh::ByteBuffer& buf);
				void _handleTYPE(anh::ByteBuffer& buf);
				void _handleROWS(anh::ByteBuffer& buf);

				std::vector<char> column_types_;
				std::vector<std::string> column_names_;
				std::list<DATA_ROW> rows_;
			};
		};
	};
};