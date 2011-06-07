#include <gtest/gtest.h>

#include "filemanager.h"
#include "iff/mock_interpreter.h"
#include "notfound_exception.h"

using namespace anh::clientfile_manager;

//I would use a mock class for this, but the filemanager interface makes it impossible to set
//expectations on it. This causes a LOT of warning messages that will scare people.
class NullInterpreter : public iff::interpreters::interpreter_interface
{
public:
	NullInterpreter()
		: interpreter_interface(), call_count_(0)
	{
	}

	virtual iff::interpreters::InterpreterType getType() { return iff::interpreters::IFF_INTERPRETER; }

	virtual void interpret(std::shared_ptr<iff::file_node> node) {
		++call_count_;
	};

	unsigned int call_count_;

};

TEST(fileManagerTests, canLoadInTreeNonSupportingDirectory)
{
	ASSERT_NO_THROW({
		file_manager f("test_data/extracted/directory1/");
	});
}

TEST(fileManagerTests, canLoadInTreeSupportingDirectory)
{
	ASSERT_NO_THROW({
		file_manager f("test_data/");
	});
}

TEST(fileManagerTests, LoadingInNonExistantDirectoryCausesException)
{
	ASSERT_THROW({
		file_manager f("test_data/this_directory_better_not_exist/");
	}, NotFoundException);
}

TEST(fileManagerTests, loadingNonExistants) 
{
	file_manager f("test_data/");

	ASSERT_NO_THROW({
		EXPECT_EQ(nullptr, f.load<iff::interpreters::MockInterpreter>("lolololololololololwut.iff"));
	});

	//For this we test with both load_directory
	ASSERT_NO_THROW({
		EXPECT_EQ(0, f.load_directory<iff::interpreters::MockInterpreter>("lolololololololololwut/").size());
	});

	//and load_all
	ASSERT_NO_THROW({
		EXPECT_EQ(0, f.load_all<iff::interpreters::MockInterpreter>("lolololololololololwut/").size());
	});
}

TEST(fileManagerTests, loadingExistantFile) 
{
	file_manager f("test_data/");

	ASSERT_NO_THROW({
		auto file = f.load<NullInterpreter>("extracted/good.iff");
	});

}

TEST(fileManagerTests, loadingWithDirectoryIsNotRecursive) 
{
	file_manager f("test_data/");

	ASSERT_NO_THROW({
		auto file = f.load_directory<NullInterpreter>("extracted/");
		EXPECT_EQ(1, file.size());
	});
}

TEST(fileManagerTests, loadingWithAllIsRecursive) 
{
	file_manager f("test_data/");

	ASSERT_NO_THROW({
		auto file = f.load_all<NullInterpreter>("extracted/");
		EXPECT_EQ(3, file.size());
	});
}

TEST(fileManagerTests, loadingWithLoadCanHaveAPostLoadLambda) 
{
	file_manager f("test_data/");

	ASSERT_NO_THROW({

		int my_counter = 0;

		auto file = f.load<NullInterpreter>("extracted/good.iff", [&] (std::shared_ptr<NullInterpreter> null, file_manager& f) {
			my_counter++;
		});

		EXPECT_EQ(1, my_counter);

	});
}

TEST(fileManagerTests, loadingWithDirectoryCanHaveAPostLoadLambda) 
{
	file_manager f("test_data/");

	//ASSERT_NO_THROW({

		int my_counter = 0;

		auto file = f.load_directory<NullInterpreter>("appearance/mesh/", "", [&] (std::shared_ptr<NullInterpreter> null, file_manager& f) {
			my_counter++;
		});

		EXPECT_EQ(3, my_counter);

	//});
}

TEST(fileManagerTests, loadingWithAllCanHaveAPostLoadLambda) 
{
	file_manager f("test_data/");

	ASSERT_NO_THROW({

		int my_counter = 0;

		auto file = f.load_all<NullInterpreter>("extracted/", "", [&] (std::shared_ptr<NullInterpreter> null, file_manager& f) {
			my_counter++;
		});

		EXPECT_EQ(3, my_counter);

	});
}

TEST(fileManagerTests, loadingWithLoadPrefersFilesOnHardDiskOverTree) 
{
	file_manager f("test_data/");

	ASSERT_NO_THROW({

		auto file = f.load<NullInterpreter>("appearance/mesh/iam13andwhatisthislolwutlolwutlolwu.msh");
		EXPECT_EQ(1, file->call_count_);

	});
}

TEST(fileManagerTests, loadingWithDirectoryPrefersFilesOnHardDiskOverTree) 
{
	file_manager f("test_data/");

	ASSERT_NO_THROW({

		auto directory1 = f.load_directory<NullInterpreter>("appearance/mesh/");
		EXPECT_EQ(1, directory1[0]->call_count_);

	});
}

TEST(fileManagerTests, loadingWithAllPrefersFilesOnHardDiskOverTree) 
{
	file_manager f("test_data/");

	ASSERT_NO_THROW({

		auto directory2 = f.load_all<NullInterpreter>("appearance/mesh/");
		EXPECT_EQ(1, directory2[0]->call_count_);

	});
}

TEST(fileManagerTests, reloadingWithLoadDoesNotPhysicallyReload) 
{
	file_manager f("test_data/");

	ASSERT_NO_THROW({

		auto file = f.load<NullInterpreter>("appearance/mesh/iam13andwhatisthislolwutlolwutlolwu.msh");

		auto reloaded_file = f.load<NullInterpreter>("appearance/mesh/iam13andwhatisthislolwutlolwutlolwu.msh");

		EXPECT_EQ(file, reloaded_file);

	});
}

TEST(fileManagerTests, reloadingWithDirectoryDoesNotPhysicallyReload) 
{
	file_manager f("test_data/");

	//ASSERT_NO_THROW({

		auto directory1 = f.load_directory<NullInterpreter>("appearance/mesh/");
		auto reloaded_directory1 = f.load_directory<NullInterpreter>("appearance/mesh/");

		ASSERT_TRUE(directory1.size() == reloaded_directory1.size());

		for(size_t i=0; i < reloaded_directory1.size(); ++i)
		{
			EXPECT_EQ(directory1[i].get(), reloaded_directory1[i].get());
		}

	//});
}

TEST(fileManagerTests, reloadingWithAllDoesNotPhysicallyReload) 
{
	file_manager f("test_data/");

	ASSERT_NO_THROW({

		auto directory1 = f.load_all<NullInterpreter>("appearance/mesh/");
		auto reloaded_directory1 = f.load_all<NullInterpreter>("appearance/mesh/");

		ASSERT_TRUE(directory1.size() == reloaded_directory1.size());

		for(size_t i=0; i < reloaded_directory1.size(); ++i)
		{
			EXPECT_EQ(directory1[i], reloaded_directory1[i]);
		}

	});
}