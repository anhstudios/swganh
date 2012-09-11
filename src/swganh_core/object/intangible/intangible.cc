// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "intangible.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::messages;

Intangible::Intangible()
	: generic_int_(0)
{
}

uint32_t Intangible::GetGenericInt()
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	return generic_int_;
}

void Intangible::SetGenericInt(uint32_t generic_int)
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	generic_int_ = generic_int;
}