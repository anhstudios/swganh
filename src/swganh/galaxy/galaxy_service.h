/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef SWGANH_GALAXY_GALAXY_SERVICE_H_
#define SWGANH_GALAXY_GALAXY_SERVICE_H_

#include "swganh/base/base_service.h"

#include "swganh/galaxy/providers/galaxy_provider_interface.h"

namespace anh {
namespace app {
class KernelInterface;
}}  // namespace anh::app


namespace swganh {
namespace galaxy {

class GalaxyService : public swganh::base::BaseService{
public:    
    explicit GalaxyService(swganh::app::SwganhKernel* kernel);
	uint32_t GetPopulation();
	anh::service::ServiceDescription GetServiceDescription();
    
private:
    GalaxyService();
	std::shared_ptr<providers::GalaxyProviderInterface> galaxy_provider_;
};

}}  // swganh::galaxy
#endif // SWGANH_GALAXY_GALAXY_SERVICE_H_
