#include <gtest/gtest.h>
#include <boost/filesystem.hpp>
#include <anh/clientfile_manager/iff/iff.h>
#include <anh/clientfile_manager/iff/filenode.h>
#include <anh/clientfile_manager/iff/foldernode.h>
#include <anh/clientfile_manager/notfound_exception.h>
#include <anh/byte_buffer.h>

#include <anh/clientfile_manager/interpreters/mock_interpreter.h>

using namespace anh::clientfile_manager::iff;
using namespace anh::clientfile_manager::interpreters;
using ::testing::_;
using ::testing::AnyNumber;

namespace
{

//This test confirms that the iff_file can properly handle being given a bad filename.
TEST(iffFileTests, NonExistantFileTest) 
{
	//Setup
	boost::filesystem::path p("./test_data/thisbetternotexist.iff");
	ASSERT_FALSE(boost::filesystem::exists(p)) << "Non existant file actually exists.";
	
	//The test
	ASSERT_THROW(iff_file file(p.file_string()), anh::clientfile_manager::NotFoundException) << "iff_file does not properly throw a NotFoundException";
}

//This test confirms that the iff_file can work properly even without an interpreter.
TEST(iffFileTests, StringInputWithOutInterpreter)
{
	//Setup
	boost::filesystem::path p("./test_data/extracted/directory1/good.iff");
	ASSERT_TRUE(boost::filesystem::exists(p)) << "Test file Does not exist.";

	//The Test
	ASSERT_NO_THROW({
		iff_file file(p.file_string());
		EXPECT_EQ(nullptr, file.interpreter()) << "The Returned interpreter is not nullptr!";

		///////BELOW HERE WE TEST FOR VALUES WE KNOW SHOULD BE TRUE FOR THIS PARTICULAR FILE///////

		//Headnode Count Check
		EXPECT_EQ(1, file.headnode_count()) << "We had too many head nodes!";

		//Headnode Name check
		std::shared_ptr<node> head_node_n = file.headnode(0);
		EXPECT_EQ(0, strcmp("FORM", head_node_n->name().c_str()));

		//Headnode type check
		EXPECT_EQ(anh::clientfile_manager::NODE_TYPE_FOLDER, head_node_n->type());
		folder_node* head_node_folder = (folder_node*)head_node_n.get();
		
		//Headnode child count check
		EXPECT_EQ(1, head_node_folder->children_count());

		std::shared_ptr<node> child_node_n = head_node_folder->child(0);
		EXPECT_EQ(anh::clientfile_manager::NODE_TYPE_FOLDER, child_node_n->type());
		folder_node* child_node_folder = (folder_node*)child_node_n.get();

		//Headnode child's children count check
		EXPECT_EQ(28, child_node_folder->children_count());
		
		for(size_t i=0; i < child_node_folder->children_count(); ++i)
		{
			std::shared_ptr<node> child_child_node_n = child_node_folder->child(i);
			EXPECT_EQ(anh::clientfile_manager::NODE_TYPE_FILE, child_child_node_n->type());
		}


	}) << "We threw someting even though the file is good.";
}

//This test confirms that an iff_file can correctly use an interpreter when opened by a string filename.
TEST(iffFileTests, StringInputWithInterpreter)
{
	//Setup
	boost::filesystem::path p("./test_data/extracted/directory1/good.iff");
	ASSERT_TRUE(boost::filesystem::exists(p)) << "Test file Does not exist.";
	MockInterpreter* moc = new MockInterpreter();
	std::shared_ptr<MockInterpreter> mock_interpreter = std::shared_ptr<MockInterpreter>(moc);
	EXPECT_CALL(*moc, interpret_data(_)).Times(28);

	//The Test
	ASSERT_NO_THROW({
		iff_file file(p.file_string(), mock_interpreter);

		EXPECT_EQ(mock_interpreter, file.interpreter()) << "The Returned interpreter is not mock_interpreter!";

		///////BELOW HERE WE TEST FOR VALUES WE KNOW SHOULD BE TRUE FOR THIS PARTICULAR FILE///////

		//Headnode Count Check
		EXPECT_EQ(1, file.headnode_count()) << "We had too many head nodes!";

		//Headnode Name check
		std::shared_ptr<node> head_node_n = file.headnode(0);
		EXPECT_EQ(0, strcmp("FORM", head_node_n->name().c_str()));

		//Headnode type check
		EXPECT_EQ(anh::clientfile_manager::NODE_TYPE_FOLDER, head_node_n->type());
		folder_node* head_node_folder = (folder_node*)head_node_n.get();
		
		//Headnode child count check
		EXPECT_EQ(1, head_node_folder->children_count());

		std::shared_ptr<node> child_node_n = head_node_folder->child(0);
		EXPECT_EQ(anh::clientfile_manager::NODE_TYPE_FOLDER, child_node_n->type());
		folder_node* child_node_folder = (folder_node*)child_node_n.get();

		//Headnode child's children count check
		EXPECT_EQ(28, child_node_folder->children_count());
		
		for(size_t i=0; i < child_node_folder->children_count(); ++i)
		{
			std::shared_ptr<node> child_child_node_n = child_node_folder->child(i);
			EXPECT_EQ(anh::clientfile_manager::NODE_TYPE_FILE, child_child_node_n->type());
		}
	}) << "We threw someting even though the file is good.";
}

//This test confirms that an iff_file can correctly use an interpreter when opened by a FILE*
TEST(iffFileTests, FilePointerInputWithInterpreter)
{
	//Setup
	boost::filesystem::path p("./test_data/extracted/directory1/good.iff");
	ASSERT_TRUE(boost::filesystem::exists(p)) << "Test file Does not exist.";

	FILE* f = fopen(p.file_string().c_str(), "rb");

	MockInterpreter* moc = new MockInterpreter();
	std::shared_ptr<MockInterpreter> mock_interpreter = std::shared_ptr<MockInterpreter>(moc);
	EXPECT_CALL(*moc, interpret_data(_)).Times(28);

	//The Test
	ASSERT_NO_THROW({
		iff_file file(f, mock_interpreter);

		EXPECT_EQ(mock_interpreter, file.interpreter()) << "The Returned interpreter is not mock_interpreter!";

		///////BELOW HERE WE TEST FOR VALUES WE KNOW SHOULD BE TRUE FOR THIS PARTICULAR FILE///////

		//Headnode Count Check
		EXPECT_EQ(1, file.headnode_count()) << "We had too many head nodes!";

		//Headnode Name check
		std::shared_ptr<node> head_node_n = file.headnode(0);
		EXPECT_EQ(0, strcmp("FORM", head_node_n->name().c_str()));

		//Headnode type check
		EXPECT_EQ(anh::clientfile_manager::NODE_TYPE_FOLDER, head_node_n->type());
		folder_node* head_node_folder = (folder_node*)head_node_n.get();
		
		//Headnode child count check
		EXPECT_EQ(1, head_node_folder->children_count());

		std::shared_ptr<node> child_node_n = head_node_folder->child(0);
		EXPECT_EQ(anh::clientfile_manager::NODE_TYPE_FOLDER, child_node_n->type());
		folder_node* child_node_folder = (folder_node*)child_node_n.get();

		//Headnode child's children count check
		EXPECT_EQ(28, child_node_folder->children_count());
		
		for(size_t i=0; i < child_node_folder->children_count(); ++i)
		{
			std::shared_ptr<node> child_child_node_n = child_node_folder->child(i);
			EXPECT_EQ(anh::clientfile_manager::NODE_TYPE_FILE, child_child_node_n->type());
		}
	}) << "We threw someting even though the file is good.";

	fclose(f);
}

//This test confirms that an iff_file can correctly use an interpreter when opened with a ByteBuffer
TEST(iffFileTests, ByteBufferInputWithInterpreter)
{
	//Setup
	boost::filesystem::path p("./test_data/extracted/directory1/good.iff");
	ASSERT_TRUE(boost::filesystem::exists(p)) << "Test file Does not exist.";

	FILE* f = fopen(p.file_string().c_str(), "rb");

	anh::ByteBuffer test;
	unsigned char buffer[1024];
	
	size_t bytes_read = fread(buffer, 1, 1024, f);
	while(bytes_read == 1024)
	{
		test.write(buffer, bytes_read);
		bytes_read = fread(buffer, 1, 1024, f);
	}
	test.write(buffer, bytes_read);

	MockInterpreter* moc = new MockInterpreter();
	std::shared_ptr<MockInterpreter> mock_interpreter = std::shared_ptr<MockInterpreter>(moc);
	EXPECT_CALL(*moc, interpret_data(_)).Times(28);

	//The Test
	ASSERT_NO_THROW({
		iff_file file(test, mock_interpreter);

		EXPECT_EQ(mock_interpreter, file.interpreter()) << "The Returned interpreter is not mock_interpreter!";

		///////BELOW HERE WE TEST FOR VALUES WE KNOW SHOULD BE TRUE FOR THIS PARTICULAR FILE///////

		//Headnode Count Check
		EXPECT_EQ(1, file.headnode_count()) << "We had too many head nodes!";

		//Headnode Name check
		std::shared_ptr<node> head_node_n = file.headnode(0);
		EXPECT_EQ(0, strcmp("FORM", head_node_n->name().c_str()));

		//Headnode type check
		EXPECT_EQ(anh::clientfile_manager::NODE_TYPE_FOLDER, head_node_n->type());
		folder_node* head_node_folder = (folder_node*)head_node_n.get();
		
		//Headnode child count check
		EXPECT_EQ(1, head_node_folder->children_count());

		std::shared_ptr<node> child_node_n = head_node_folder->child(0);
		EXPECT_EQ(anh::clientfile_manager::NODE_TYPE_FOLDER, child_node_n->type());
		folder_node* child_node_folder = (folder_node*)child_node_n.get();

		//Headnode child's children count check
		EXPECT_EQ(28, child_node_folder->children_count());
		
		for(size_t i=0; i < child_node_folder->children_count(); ++i)
		{
			std::shared_ptr<node> child_child_node_n = child_node_folder->child(i);
			EXPECT_EQ(anh::clientfile_manager::NODE_TYPE_FILE, child_child_node_n->type());
		}
	}) << "We threw someting even though the file is good.";

	fclose(f);
}

TEST(iffFileTests, fileNodeLoadsDataProperly)
{
	//SETUP
	anh::ByteBuffer buffer(20);

	buffer.write<std::uint32_t>(0xDEADBABE);
	buffer.write<std::uint32_t>(0xDEADBEEF);
	buffer.write<std::uint32_t>(0xCAFEBABE);
	buffer.write<std::uint32_t>(0x5CA1AB1E);
	buffer.write<std::uint32_t>(0xDEADBEA7);

	file_node file(std::string("TEST"), 20);
	file.loadData(buffer);

	//TESTING

	//Is name/size correct?
	EXPECT_EQ(0, strcmp("TEST", file.name().c_str()));
	EXPECT_EQ(20, file.size());

	//are the buffers at the right positions?
	EXPECT_EQ(20, buffer.read_position());
	EXPECT_EQ(20, file.data().write_position());

	//is the data copried right?
	EXPECT_EQ(0xDEADBABE, file.data().read<std::uint32_t>());
	EXPECT_EQ(0xDEADBEEF, file.data().read<std::uint32_t>());
	EXPECT_EQ(0xCAFEBABE, file.data().read<std::uint32_t>());
	EXPECT_EQ(0x5CA1AB1E, file.data().read<std::uint32_t>());
	EXPECT_EQ(0xDEADBEA7, file.data().read<std::uint32_t>());

}

TEST(iffFileTests, folderNodeMethodsWork)
{
	//For the purposes of testing folder_nodes, size is not really significant.
	folder_node test(std::string("TEST"), 0);

	//We should start with 0 children
	EXPECT_EQ(0, test.children_count());

	test.add(std::make_shared<folder_node>(std::string("TEST"), 0xDEADBABE));
	test.add(std::make_shared<folder_node>(std::string("TEST"), 0xDEADBEEF));
	test.add(std::make_shared<folder_node>(std::string("TEST"), 0xCAFEBABE));
	test.add(std::make_shared<folder_node>(std::string("TEST"), 0x5CA1AB1E));
	test.add(std::make_shared<folder_node>(std::string("TEST"), 0xDEADBEA7));

	//Do we have the right number of children?
	EXPECT_EQ(5, test.children_count());

	//We will tell our children apart by their sizes.
	//Did they get added in the right order/no duplicates

	EXPECT_EQ(0xDEADBABE, test.child(0)->size());
	EXPECT_EQ(0xDEADBEEF, test.child(1)->size());
	EXPECT_EQ(0xCAFEBABE, test.child(2)->size());
	EXPECT_EQ(0x5CA1AB1E, test.child(3)->size());
	EXPECT_EQ(0xDEADBEA7, test.child(4)->size());

	test.remove(4);
	test.remove(3);
	test.remove(2);
	test.remove(1);
	test.remove(0);

	//We should end with 0 children
	EXPECT_EQ(0, test.children_count());
}

};