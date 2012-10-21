// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "factory_crate.h"

using namespace swganh::object;

uint32_t FactoryCrate::GetType() const 
{ 
    return FactoryCrate::type; 
}

std::shared_ptr<Object> FactoryCrate::Clone()
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	auto other = std::make_shared<FactoryCrate>();
	Clone(other);
	return other;
}

void FactoryCrate::Clone(std::shared_ptr<FactoryCrate> other)
{
	Tangible::Clone(other);
}