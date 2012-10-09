// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "factory_crate_factory.h"

#include "factory_crate.h"

using namespace std;
using namespace swganh::object;

FactoryCrateFactory::FactoryCrateFactory(swganh::app::SwganhKernel* kernel)
	: TangibleFactory(kernel)
{
}