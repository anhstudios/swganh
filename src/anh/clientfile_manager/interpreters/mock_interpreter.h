
#include <anh/clientfile_manager/interpreters/interpreter_interface.h>
#include <gmock/gmock.h>

namespace anh
{
	namespace clientfile_manager
	{
		namespace interpreters
		{
			class MockInterpreter : public interpreter_interface
			{
			public:
				MOCK_METHOD0(getType, InterpreterType());
				MOCK_METHOD1(interpret_data, void(std::shared_ptr<iff::file_node> node));
				MOCK_METHOD1(interpret_folder, void(std::shared_ptr<iff::folder_node> node));
			};
		};
	};
};