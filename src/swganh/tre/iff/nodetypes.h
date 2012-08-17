// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef NODE_TYPES_H
#define NODE_TYPES_H

namespace swganh
{
namespace tre
{
	/**
		The possible node types. Used for casting Nodes from/to the base class.
	*/
	enum NODE_TYPE
	{
		//You should never see this nodetype. It is here for completeness
		NODE_TYPE_BASE,
		
		//These are the default types. 
		//They mean that no specialized type could be found.
		NODE_TYPE_FOLDER,
		NODE_TYPE_FILE,
	};
}
}
#endif