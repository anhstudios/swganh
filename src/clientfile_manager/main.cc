#include <iostream>
#include <string>
#include "main.h"
#include "iff/iff.h"
#include "filemanager.h"
#include "iff/interpreters/POB/POBInterpreter.h"
#include "iff/interpreters/WS/WSInterpreter.h"
#include "iff/interpreters/OIFF/OIFFInterpreter.h"

using namespace ClientFileManager;

int main(int argc, char* argv[])
{
	std::string s;
	printf("Input SWG Directory (contains swg2uu_live.cfg): ");
	std::getline(std::cin, s);

	//First we must initialize the FileManager.
	FileManager f(s);



	printf("Loading Player Race Templates...\n");
	auto playerRaces = f.loadDirectory<IFF::Interpreters::OIFFInterpreter>("object/creature/player/", [] (std::shared_ptr<IFF::Interpreters::OIFFInterpreter> obj, FileManager& f) {
		obj->loadAggregateData(f);
	});

	printf("Loading Building Templates...\n");
	auto buildings = f.loadAll<IFF::Interpreters::OIFFInterpreter>("object/building/", [] (std::shared_ptr<IFF::Interpreters::OIFFInterpreter> obj, FileManager& f) {
		obj->loadAggregateData(f);
	});
}

/*
bool API Load(std::shared_ptr<anh::module_manager::PlatformServices> services) {
	return true;
}

bool API Unload(std::shared_ptr<anh::module_manager::PlatformServices> services) {
	return true;
}

const std::string API GetModuleName(void) {
	return "DLLTest";
}

const anh::module_manager::ModuleApiVersion API GetModuleVersion(void) {
	return anh::module_manager::ModuleApiVersion(0, 1, "0.1");
}

const std::string API GetModuleDescription(void) {
	return "A test module.";
}
*/