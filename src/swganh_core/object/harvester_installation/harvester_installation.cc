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
