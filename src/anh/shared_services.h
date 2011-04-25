#include <anh/module_manager/platform_services.h>
#include <anh/module_manager/module_interface.h>
#include <anh/event_dispatcher/event_dispatcher_interface.h>
#include <anh/component/object_manager.h>
#include <anh/component/object_builder.h>
#include <anh/database/database_manager_interface.h>
#include <anh/scripting/scripting_manager_interface.h>
#include <anh/server_directory/server_directory_interface.h>
using namespace anh;
using namespace std;
// global shared
typedef shared_ptr<event_dispatcher::EventDispatcherInterface>				gEventDispatcher;
typedef shared_ptr<component::ObjectBuilder>								gObjectCreator;
typedef shared_ptr<component::ObjectManager>								gObjectMgr;
typedef shared_ptr<database::DatabaseManagerInterface>						gDatabaseMgr;
typedef shared_ptr<scripting::ScriptingManagerInterface>					gScriptingMgr;
typedef shared_ptr<server_directory::ServerDirectoryInterface>				gServerDirectory;
