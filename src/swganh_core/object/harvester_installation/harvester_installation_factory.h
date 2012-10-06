// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh_core/object/installation/installation_factory.h"

namespace swganh {
namespace object {

	class HarvesterInstallation;
    class HarvesterInstallationFactory : public swganh::object::InstallationFactory
    {
    public:
		typedef HarvesterInstallation ObjectType;
		virtual void PersistChangedObjects(){}
        HarvesterInstallationFactory(swganh::app::SwganhKernel* kernel);
    };

}}  // namespace swganh::object
