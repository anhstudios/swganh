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
	FileManager f(s);

	auto pob = std::make_shared<IFF::Interpreters::POBInterpreter>();
	auto ws = std::make_shared<IFF::Interpreters::WSInterpreter>();
	auto oiff = std::make_shared<IFF::Interpreters::OIFFInterpreter>();
	try
	{
		f.getFile("appearance/ply_nboo_assoc_hall_civ_s01.pob", pob);
		f.getFile("snapshot/naboo.ws", ws);
		f.getFile("object/building/naboo/shared_cantina_naboo.iff", oiff);
	}
	catch(IFF::NotFoundException*)
	{
		printf("That file does not exist!");
	}
	//pob->debug();
	//ws->debug();
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