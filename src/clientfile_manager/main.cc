#include "main.h"

#include "iff/iff.h"

using namespace ClientFileManager::IFF;

int main(int argc, char* argv[])
{
	IFFFile iff("I:\\SWGDATA\\appearance\\ply_tato_house_sm_s01_fp1.pob");
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