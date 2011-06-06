
#include "interpreters/interpreter_interface.h"
#include <gmock/gmock.h>

namespace anh
{
	namespace clientfile_manager
	{
		namespace iff
		{
			namespace interpreters
			{
				class MockInterpreter : public interpreter_interface
				{
				public:
					MOCK_METHOD0(getType, InterpreterType());
					MOCK_METHOD1(interpret, void(std::shared_ptr<iff::file_node> node));
				};
			};
		};
	};
};