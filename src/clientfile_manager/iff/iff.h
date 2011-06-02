/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2011 The SWG:ANH Team
---------------------------------------------------------------------------------------
Use of this source code is governed by the GPL v3 license that can be found
in the COPYING file or at http://www.gnu.org/licenses/gpl-3.0.html

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,a
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
---------------------------------------------------------------------------------------
*/

#ifndef IFF_H
#define IFF_H

#include <stdio.h>
#include <vector>
#include <map>
#include <cstdint>
#include <memory>
#include <functional>
#include <exception>

#include <anh/byte_buffer.h>

namespace ClientFileManager
{
	namespace IFF
	{
		class Node;
		namespace Interpreters
		{
			class IFFInterpreter;
		};

		typedef std::function<std::shared_ptr<IFF::Node>(std::shared_ptr<std::string>, std::uint32_t)> IFFCreatorCallback;
		typedef std::map<std::string, IFFCreatorCallback> NodeIndex;
		typedef std::map<std::string, IFFCreatorCallback>::iterator NodeIndexIterator;

		class NotFoundException : public std::exception
		{
		public:
			NotFoundException(std::string message)
				: message_(message)
			{
			}
			
			virtual const char* what() const throw() { return message_.c_str();} 
		private:
			std::string message_;
		};
		class BadIFFException : public std::exception
		{
		public:
			BadIFFException(std::string message)
				: message_(message)
			{
			}

			virtual const char* what() const throw() { return message_.c_str(); }
		private:
			std::string message_;
		};

		class IFFFile
		{
		public:
			IFFFile(IFFFile&& other);
			IFFFile& operator=(IFFFile&& other);
			IFFFile(std::string filename, std::shared_ptr<Interpreters::IFFInterpreter> interpret);
			IFFFile(FILE* inputstream, std::shared_ptr<Interpreters::IFFInterpreter> interpret);
			IFFFile(anh::ByteBuffer& input, std::shared_ptr<Interpreters::IFFInterpreter> interpret);

			std::shared_ptr<Interpreters::IFFInterpreter> getInterpreter() { return interpreter_;}
			std::shared_ptr<ClientFileManager::IFF::Node> headnode(unsigned int i) {return headNodes[i];}
			unsigned int headnode_count() {return headNodes.size();}

			

		protected:
		private:
			void _loadIFF(FILE* stream);
			void _loadIFF(anh::ByteBuffer& inputstream);
			std::vector<std::shared_ptr<ClientFileManager::IFF::Node>> headNodes;
			std::shared_ptr<Interpreters::IFFInterpreter> interpreter_;
		};
	};
};

#endif