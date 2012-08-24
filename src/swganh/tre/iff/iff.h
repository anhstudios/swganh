// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef IFF_H
#define IFF_H

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
	class iff_file
	{
	public:
		//Constructors
		iff_file(anh::ByteBuffer input, std::shared_ptr<VisitorInterface> interpret=nullptr);

	private:
		void loadIFF_(anh::ByteBuffer& inputstream);
		bool isNodeNameCharacter_(char c);
		unsigned int getNameSize_(char namedata[], unsigned int size);
		bool isFolderNode_(std::string& name);
		std::string getIFFName_(anh::ByteBuffer& input);

		std::shared_ptr<VisitorInterface> visitor_;
	};
}
}
#endif