#include "memory_search.h"
#include "global.h"
#include "menu.h"
#include "hooks.h"

void __test() {
	
}

void init() {
	memory_search_init();

	//gEngfuncs->pfnAddCommand((char*)"__unhook", memory_search_release);
	//gEngfuncs->pfnAddCommand((char*)"__test", __test);
}

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		module_instance_handle = instance;
		init();
	}

	return TRUE;
}