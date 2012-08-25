// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <stdio.h>
#include <vector>
#include <map>
#include <cstdint>
#include <memory>
#include <functional>

#include <anh/byte_buffer.h>
#include <swganh/tre/visitors/visitor_interface.h>


namespace swganh
{
namespace tre
{
	class node;

	class iff_file
	{
	public:
		//Constructors
		iff_file(anh::ByteBuffer input, std::shared_ptr<VisitorInterface> interpret=nullptr);

		/**
			@return the interpreter associated with this IFFFile
		*/
		std::shared_ptr<VisitorInterface> interpreter() { return visitor_;}
			
		/**
			@param i the index of the headnode to remove
			@return a head node
		*/
		std::shared_ptr<node> headnode(unsigned int i) {return headNodes[i];}

		/**
			@return the count of headnodes
		*/
		unsigned int headnode_count() {return headNodes.size();}

		//Move constructor
		iff_file& operator=(iff_file&& other);

	protected:
	private:
		void loadIFF_(anh::ByteBuffer& inputstream);
		bool isNodeNameCharacter_(char c);
		unsigned int getNameSize_(char namedata[], unsigned int size);
		bool isFolderNode_(std::string& name);
		std::string getIFFName_(anh::ByteBuffer& input);

		std::vector<std::shared_ptr<node>> headNodes;
		std::shared_ptr<VisitorInterface> visitor_;
	};
}
}
