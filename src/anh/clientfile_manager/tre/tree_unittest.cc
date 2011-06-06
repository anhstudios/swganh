#include <gtest/gtest.h>
#include "tree.h"
#include "tree_entry.h"
#include <anh/byte_buffer.h>

using namespace anh::clientfile_manager::tree;

TEST(treeArchiveTests, loadingANonExistingTreeDoesNothing) 
{
	std::map<std::string, const tree_entry> entries;
	tree_archive(std::string("test_data/LOLWUT.tre"), entries);
	EXPECT_EQ(0, entries.size());
}

TEST(treeArchiveTests, loadingARealTreeDoesSomething) 
{
	std::map<std::string, const tree_entry> entries;
	tree_archive(std::string("test_data/test_tree.tre"), entries);
	EXPECT_NE(0, entries.size());
}

//please refer to the filemanager_unittest.cc for other .tre releated test cases (such as data extraction)