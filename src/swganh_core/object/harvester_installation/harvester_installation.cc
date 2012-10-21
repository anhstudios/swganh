// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "harvester_installation.h"

using namespace swganh::object;

HarvesterInstallation::HarvesterInstallation()
	: Installation()
{
}

uint32_t HarvesterInstallation::GetType() const
{ 
    return HarvesterInstallation::type; 
}


std::shared_ptr<Object> HarvesterInstallation::Clone()
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	auto other = std::make_shared<HarvesterInstallation>();
	Clone(other);
	return other;
}

void HarvesterInstallation::Clone(std::shared_ptr<HarvesterInstallation> other)
{
	Installation::Clone(other);
}