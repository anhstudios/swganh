#include "GameSystems_init.h"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include "swganh/plugin/bindings.h"
#include "swganh/plugin/plugin_manager.h"

#include "swganh/app/swganh_kernel.h"

#include "GameSystems_service.h"

#include "version.h"

namespace swganh
{
namespace gamesystems
{

void Initialize(swganh::app::SwganhKernel* kernel)
{
    swganh::plugin::ObjectRegistration registration;
    registration.version.major = VERSION_MAJOR;
    registration.version.minor = VERSION_MINOR;

    registration.CreateObject = [kernel] (swganh::plugin::ObjectParams* params) -> void *
    {
        return new GameSystemsService(kernel);
    };

    registration.DestroyObject = [] (void* object)
    {
        if(object)
        {
            delete static_cast<GameSystemsService*>(object);
        }
    };
	
    kernel->GetPluginManager()->RegisterObject("GameSystems::GameSystemsService", &registration);
}

}
} // namespace swganh::gamesystem