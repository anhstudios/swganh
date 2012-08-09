// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef OBJECT_VISITOR_INTL_H
#define OBJECT_VISITOR_INTL_H

namespace swganh
{
namespace tre
{
	template <class T> std::shared_ptr<T> ObjectVisitor::attribute(std::string& key)
	{
		std::map<std::string, boost::any>::const_iterator it = attributes_.find(key);
		if(it != attributes_.cend())
		{
			return boost::any_cast<std::shared_ptr<T>>(it->second);
		}
		return nullptr;
	}
}
}

#endif