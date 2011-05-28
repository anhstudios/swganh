#include "main.h"

#include "iff.h"

using namespace ClientFileManager::IFF;

int main(int argc, char* argv[])
{
	IFFFile iff("I:\\SWGDATA\\appearance\\mesh\\mun_corl_capitol_s01_r0_mesh_l0_c7_l0_c7.msh");
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