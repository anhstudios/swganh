#include <iostream>
#include <string>
#include "main.h"
#include "iff/iff.h"

#include "iff/interpreters/POBInterpreter.h"

using namespace ClientFileManager::IFF;

int main(int argc, char* argv[])
{
	std::string s;
	printf("Filename: ");
	std::getline(std::cin, s);

	Interpreters::POBInterpreter* pob = new Interpreters::POBInterpreter();

	try
	{
		IFFFile iff(s, pob);
	}
	catch(NotFoundException* e)
	{
		printf("That file does not exist!");
	}

	pob->debug();

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