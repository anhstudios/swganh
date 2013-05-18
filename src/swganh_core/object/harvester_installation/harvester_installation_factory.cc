// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "harvester_installation_factory.h"

#include "harvester_installation.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::object;

HarvesterInstallationFactory::HarvesterInstallationFactory(swganh::app::SwganhKernel* kernel)
	: InstallationFactory(kernel)
{}

void HarvesterInstallationFactory::LoadFromStorage(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Object>& object, boost::unique_lock<boost::mutex>& lock)
{
    InstallationFactory::LoadFromStorage(connection, object, lock);
}

std::shared_ptr<Object> HarvesterInstallationFactory::CreateObject()
{
	return std::make_shared<HarvesterInstallation>();
}