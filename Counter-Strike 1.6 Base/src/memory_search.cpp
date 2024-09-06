#include "memory_search.h"
#include "memory.h"
#include "messages.h"
#include "global.h"
#include <Windows.h>
#include "hooks.h"

Module hw_module, client_module;

HookFunctionInfo SwapBuffers_hook_function_info;
HookFunctionInfo CL_GetCDKeyHash_hook_function_info;
HookFunctionInfo R_ForceCVars_hook_function_info;
HookCallInfo CL_ReadDemoMessage_OLD_ValidStuffText_hook_call_info;
HookCallInfo CL_ReadDemoMessage_OLD_Cbuf_AddFilteredText_hook_call_info;
HookCallInfo CL_ReadPackets_CL_ConnectionlessPacket_hook_call_info;
HookCallInfo CL_BatchResourceRequest_IsSafeFileToDownload_hook_call_info;
HookCallInfo Netchan_CopyFileFragments_IsSafeFileToDownload_hook_call_info;
HookCallInfo CL_SendConsistencyInfo_MD5_Hash_File_hook_call_info;
HookCallInfo CL_SendConnectPacket_Steam_GSInitiateGameConnection_hook_call_info;
HookCallInfo Host_Frame__Host_Frame_hook_call_info;

void load_modules() {
	if (!load_module("hw.dll", hw_module))
		return Error("Failed to load \"hw.dll\".");

	if (!load_module("client.dll", client_module))
		return Error("Failed to load \"client.dll\".");
}

void search_cl_funcs() {
	const unsigned char pattern[] = "\x40\xEF\x45\x11\xA1\x5C\xEF\x45\x11\x83\xC4\x18\x85\xC0\x74\x0A\x68\x20\xEA\x40\x11\xFF";
	const char mask[] = "????x????xxxxxx?x????x";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"cl_funcs\".");

	cl_funcs = *(cldll_func_t**)p;
}

void search_gEngfuncs() {
	const unsigned char pattern[] = "\x20\x79\x11\x10\xFF\x52\x04\x8B\x0D\xC8\xC2\x11\x10\x68";
	const char mask[] = "????xx?xx????x";

	void* p = find_pattern(client_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"gEngfuncs\".");

	gEngfuncs = *(cl_enginefunc_t**)p;
}

void search_cl_parsefuncs() {
	const unsigned char pattern[] = "\x3C\xA2\x31\x10\x8B\x0C\xB5\xA0\xE8\x24\x11\x85\xC9\x74\x48\x83";
	const char mask[] = "????xx?????xxx?x";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"cl_parsefuncs\".");

	cl_parsefuncs = (svc_func_t*)((DWORD_PTR)*(void**)p - 4);
}

void* hook_svc_function(const char* const name, void* const new_functions_address) {
	if (name == nullptr || new_functions_address == nullptr)
		return nullptr;

	svc_func_t* p = cl_parsefuncs;

	if (p == nullptr)
		return nullptr;

	void* backup;

	while (strcmp(p->pszname, "End of List") != 0) {
		if (stricmp(p->pszname, name) == 0) {
			backup = p->pfnParse;

			p->pfnParse = new_functions_address;

			return backup;
		}

		++p;
	}

	return nullptr;
}

void search_msg_readcount() {
	const unsigned char pattern[] = "\x84\xC9\x24\x11\x48\x50\x68\x50\x84\x2B\x10\xE8\xFB\x3F\x01\x00\xF3\x0F\x10\x05\x4C\xB5\x31\x10\x83\xC4\x10\x0F\x57";
	const char mask[] = "????xxx????x????xxxx????xxxxx";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"msg_readcount\".");

	msg_readcount = *(int**)p;
}

void search_MSG_ReadString() {
	const unsigned char pattern[] = "\xA6\x00\x01\x00\x8B\xF0\xE8\x4F\x34\x0A\x00\x85";
	const char mask[] = "????xxx????x";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"MSG_ReadString\".");

	MSG_ReadString = (char* (*)(void))Absolute(p);
}

void search_MSG_ReadByte() {
	const unsigned char pattern[] = "\x59\xFB\x00\x00\x8B\xF0\x83\xFE\x40";
	const char mask[] = "????xxxxx";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"MSG_ReadByte\".");

	MSG_ReadByte = (int (*)(void))Absolute(p);
}

void search_Cmd_TokenizeString() {
	const unsigned char pattern[] = "\x3F\x0D\x02\x00\x83\xC4\x18\xE8\x57\x06\x02\x00\x8B\x4D\xFC\x33\xC0";
	const char mask[] = "????xxxx????xx?xx";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"Cmd_TokenizeString\".");

	Cmd_TokenizeString = (void (*)(char*))Absolute(p);
}

void search_cls() {
	const unsigned char pattern[] = "\xE0\x8D\x40\x11\x03\x75\x2A\x83\x3D\xC4\xE6\x40\x11\x00\x0F\x85\x7C\x08\x00\x00\x68";
	const char mask[] = "????xx?xx????xxx????x";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"cls\".");

	cls = *(client_static_t**)p;
}

void search_CL_RecordHUDCommand() {
	const unsigned char pattern[] = "\xC5\x45\xFE\xFF\x83\xC4\x04\x5F\x5E\x5B\x8B\x4D\xFC\x33\xCD\xE8\x5C\x56\x0F\x00\x8B\xE5\x5D\xC3\xFF";
	const char mask[] = "????xxxxxxxx?xxx????xxxxx";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"CL_RecordHUDCommand\".");

	CL_RecordHUDCommand = (void (*)(char*))Absolute(p);
}

void search_COM_Parse() {
	const unsigned char pattern[] = "\x66\x7E\xFF\xFF\x68\x80\xC1\x24\x11\x89\x07\xE8\xFA\xA2\xFF\xFF\x83\xC4\x08\x85\xC0\x0F";
	const char mask[] = "????x????xxx????xxxxxx";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"COM_Parse\".");

	COM_Parse = (char* (*)(char*))Absolute(p);
}

void search_com_token() {
	const unsigned char pattern[] = "\x80\xC1\x24\x11\x89\x07\xE8\xFA\xA2\xFF\xFF\x83\xC4\x08\x85\xC0\x0F";
	const char mask[] = "????xxx????xxxxxx";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"com_token\".");

	com_token = *(char**)p;
}

void search_nMax() {
	const unsigned char pattern[] = "\x60\x7F\x25\x11\x83\xC4\x04\x69\xC0";
	const char mask[] = "????xxxxx";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"nMax\".");

	nMax = *(client_state_t**)p;
}

void search_Cvar_DirectSet() {
	const unsigned char pattern[] = "\x9F\xFA\xFF\xFF\xA1\xC8\x74\x4B\x10\x83\xC4\x08\x85\xC0\x74\x1A\x0F";
	const char mask[] = "????x????xxxxxx?x";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"Cvar_DirectSet\".");

	Cvar_DirectSet = (void (*)(cvar_s*, char*))Absolute(p);
}

void search_Cmd_ForwardToServer() {
	const unsigned char pattern[] = "\x55\x8B\xEC\x51\x8D\x45\xFC\xC7";
	const char mask[] = "xxxxxx?x";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"Cmd_ForwardToServer\".");

	Cmd_ForwardToServer = (void (*)(void))p;
}

void search_MSG_BeginReading() {
	const unsigned char pattern[] = "\xEC\xE1\xFD\xFF\xFF\x76\x18\xFF\x76\x10\x68\xE0\x5F\x24\x11\xE8\x5C\x03\xFE\xFF\x83\xC4\x10";
	const char mask[] = "????xx?xx?x????x????xxx";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"MSG_BeginReading\".");

	MSG_BeginReading = (void (*)(void))Absolute(p);
}

void search_MSG_ReadLong() {
	const unsigned char pattern[] = "\x4B\xF7\x00\x00\x83\xF8\x30\x74\x10\x6A";
	const char mask[] = "????xxxx?x";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"MSG_ReadLong\".");

	MSG_ReadLong = (int (*)(void))Absolute(p);
}

void search_MSG_ReadStringLine() {
	const unsigned char pattern[] = "\x5A\x80\x01\x00\x8B\xF0\x56\xE8\x42\x4A\x01\x00\x6A\x00\xE8\x1B\x37\x01\x00\x83";
	const char mask[] = "????xxxx????xxx????x";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"MSG_ReadStringLine\".");

	MSG_ReadStringLine = (char* (*)(void))Absolute(p);
}

void search_Cbuf_AddText() {
	const unsigned char pattern[] = "\x86\x61\xFE\xFF\x83\xC4\x04\xE8\xCE\x61\xFE\xFF\x8B\x0D";
	const char mask[] = "????xxxx????xx";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"Cbuf_AddText\".");

	Cbuf_AddText = (void (*)(char*))Absolute(p);
}

void search_CL_Disconnect() {
	const unsigned char pattern[] = "\x37\x91\x00\x00\x6A\x00\xE8\x80\x81\x02\x00\x83\xC4\x04\x68\xFB";
	const char mask[] = "????xxx????xxxxx";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"CL_Disconnect\".");

	CL_Disconnect = (void (*)(void))Absolute(p);
}

void search_MSG_WriteByte() {
	const unsigned char pattern[] = "\x3F\x43\x02\x00\xFF\x75\x0C\xFF\x75\x08\x68\x60\x44\x2B\x10\xE8\x1F\x50\x02\x00\x50";
	const char mask[] = "????xx?xx?x????x????x";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"MSG_WriteByte\".");

	MSG_WriteByte = (void (*)(sizebuf_t*, int))Absolute(p);
}

void search_MSG_WriteLong() {
	const unsigned char pattern[] = "\x20\xE8\xFD\xFF\x56\x8D\x85\x50\xE8\xFF\xFF\x68\xB0\x98\x4B\x10\x50\xE8\x9E\xF3\xFD\xFF\x83\xC4\x34";
	const char mask[] = "????xxx????x????xx????xxx";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"MSG_WriteLong\".");

	MSG_WriteLong = (void (*)(sizebuf_t*, int))Absolute(p);
}

void search_MSG_WriteString() {
	const unsigned char pattern[] = "\xE0\x02\x01\x00\x83\xC4\x08\x5E\xC3\xB8";
	const char mask[] = "????xxxxxx";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"MSG_WriteString\".");

	MSG_WriteString = (void (*)(sizebuf_t*, const char*))Absolute(p);
}

void search_gClientUserMsgs() {
	const unsigned char pattern[] = "\x18\xF0\x32\x10\x85\xF6\x74\x0C\x90\x39";
	const char mask[] = "????xxx?xx";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"gClientUserMsgs\".");

	gClientUserMsgs = *(UserMsg***)p;
}

void search_MSG_ReadShort() {
	const unsigned char pattern[] = "\x87\xDA\xFD\xFF\x89\x84\x35\x48\xFF\xFF\xFF\xE8\x7B\xDA\xFD\xFF\x89\x84\x35\x28\xFF\xFF\xFF\x83";
	const char mask[] = "????xxx????x????xxx????x";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"MSG_ReadShort\".");

	MSG_ReadShort = (int (*)(void))Absolute(p);
}

void search_MSG_ReadCoord() {
	const unsigned char pattern[] = "\x66\xAF\x00\x00\xD9\x5D\xD8\x68\xE0\x5F\x24\x11\xE8\x59\xAF\x00\x00\xD9\x5D\xDC\x68\xE0\x5F\x24\x11\xE8\x4C\xAF\x00\x00\x83\xC4\x0C\xEB";
	const char mask[] = "????xx?x????x????xx?x????x????xxxx";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"MSG_ReadCoord\".");

	MSG_ReadCoord = (float (*)(void))Absolute(p);
}

void search_MSG_ReadAngle() {
	const unsigned char pattern[] = "\xF4\x95\x00\x00\xD9\x5D\xDC\xC7\x45";
	const char mask[] = "????xx?xx";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"MSG_ReadAngle\".");

	MSG_ReadAngle = (float (*)(void))Absolute(p);
}

void search_cmd_args() {
	const unsigned char pattern[] = "\x30\x5E\x4A\x10\xEB\x05\x68\x40\x37\x2B\x10\x50\xE8\xA5\x50\x00\x00\x83\xC4\x08\xF6";
	const char mask[] = "????x?x????xx????xxxx";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"cmd_args\".");

	cmd_args = *(char***)p;
}

pfnUserMsgHook hook_user_msg(const char* const name, const pfnUserMsgHook new_functions_address) {
	if (name == nullptr || new_functions_address == nullptr)
		return nullptr;

	UserMsg* p = *gClientUserMsgs;

	if (p == nullptr)
		return nullptr;

	pfnUserMsgHook backup;

	while (p != nullptr) {
		if (stricmp(p->szName, name) == 0) {
			backup = p->pfn;

			p->pfn = new_functions_address;

			return backup;
		}

		p = p->next;
	}

	return nullptr;
}

void hook_SwapBuffers() {
	HMODULE gdi32_module_handle = GetModuleHandle("gdi32.dll");

	if (gdi32_module_handle == NULL)
		return Error("Failed to load \"gdi32.dll\".");

	void* p = GetProcAddress(gdi32_module_handle, "SwapBuffers");

	if (p == nullptr)
		return Error("Failed to find \"SwapBuffers\".");

	hook_function(p, SwapBuffers_hook, 6, &SwapBuffers_hook_function_info);

	original_SwapBuffers = (BOOL (WINAPI *)(HDC))SwapBuffers_hook_function_info.original_function_address;
}

void hook_CL_ReadDemoMessage_OLD_ValidStuffText() {
	const unsigned char pattern[] = "\xE8\x2B\x0B\x01\x00\x83\xC4\x14\x85\xC0\x0F\x84\x30\xFE\xFF\xFF\x8D";
	const char mask[] = "x????xxxxxxx????x";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"ValidStuffText\" inside \"CL_ReadDemoMessage_OLD\".");

	hook_call(p, CL_ReadDemoMessage_OLD_ValidStuffText_hook, &CL_ReadDemoMessage_OLD_ValidStuffText_hook_call_info);

	original_ValidStuffText = (qboolean (*)(const char*))CL_ReadDemoMessage_OLD_ValidStuffText_hook_call_info.function_address;
}

void hook_CL_ReadDemoMessage_OLD_Cbuf_AddFilteredText() {
	const unsigned char pattern[] = "\xE8\x37\xAD\x01\x00\x68\x40\x37\x2B\x10\xE8\x2D\xAD\x01\x00\x83\xC4\x08\xE9";
	const char mask[] = "x????x????x????xxxx";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"Cbuf_AddFilteredText\" inside \"CL_ReadDemoMessage_OLD\".");

	hook_call(p, CL_ReadDemoMessage_OLD_Cbuf_AddFilteredText_hook, &CL_ReadDemoMessage_OLD_Cbuf_AddFilteredText_hook_call_info);

	original_Cbuf_AddFilteredText = (void (*)(char*))CL_ReadDemoMessage_OLD_Cbuf_AddFilteredText_hook_call_info.function_address;
}

void hook_CL_ReadPackets_CL_ConnectionlessPacket() {
	const unsigned char pattern[] = "\xE8\xBA\xD0\xFF\xFF\xEB\xAE\x83\x3D\xF0\xE9\x40\x11\x00\x0F";
	const char mask[] = "x????x?xx????xx";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"CL_ConnectionlessPacket\" inside \"CL_ReadPackets\".");

	hook_call(p, CL_ReadPackets_CL_ConnectionlessPacket_hook, &CL_ReadPackets_CL_ConnectionlessPacket_hook_call_info);

	original_CL_ConnectionlessPacket = (void (*)(void))CL_ReadPackets_CL_ConnectionlessPacket_hook_call_info.function_address;
}

void hook_CL_BatchResourceRequest_IsSafeFileToDownload() {
	const unsigned char pattern[] = "\xE8\x5E\xEB\x05\x00\x83\xC4\x04\x85\xC0\x75\x1C\x56\xE8\x71\x24\x00\x00\x56";
	const char mask[] = "x????xxxxxx?xx????x";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"IsSafeFileToDownload\" inside \"CL_BatchResourceRequest\".");

	hook_call(p, CL_BatchResourceRequest_IsSafeFileToDownload_hook, &CL_BatchResourceRequest_IsSafeFileToDownload_hook_call_info);

	original_IsSafeFileToDownload = (qboolean (*)(const char*))CL_BatchResourceRequest_IsSafeFileToDownload_hook_call_info.function_address;
}

void hook_Netchan_CopyFileFragments_IsSafeFileToDownload() {
	const unsigned char pattern[] = "\xE8\xCE\x9B\x02\x00\x83\xC4\x04\x85\xC0\x75\x07\x68\x5C\xFB\x2B\x10\xEB";
	const char mask[] = "x????xxxxxx?x????x";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"IsSafeFileToDownload\" inside \"Netchan_CopyFileFragments\".");

	hook_call(p, Netchan_CopyFileFragments_IsSafeFileToDownload_hook, &Netchan_CopyFileFragments_IsSafeFileToDownload_hook_call_info);

	original_IsSafeFileToDownload = (qboolean (*)(const char*))Netchan_CopyFileFragments_IsSafeFileToDownload_hook_call_info.function_address;
}

void hook_CL_SendConsistencyInfo_MD5_Hash_File() {
	const unsigned char pattern[] = "\xE8\xB0\x46\x01\x00\x8B\x45\xEC\x33\xC9\x85";
	const char mask[] = "x????xx?xxx";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"MD5_Hash_File\" inside \"CL_SendConsistencyInfo\".");

	hook_call(p, CL_SendConsistencyInfo_MD5_Hash_File_hook, &CL_SendConsistencyInfo_MD5_Hash_File_hook_call_info);

	original_MD5_Hash_File = (BOOL (*)(unsigned char*, char*, BOOL, BOOL, unsigned int*))CL_SendConsistencyInfo_MD5_Hash_File_hook_call_info.function_address;
}

void hook_CL_GetCDKeyHash() {
	const unsigned char pattern[] = "\x55\x8B\xEC\x81\xEC\x74\x01";
	const char mask[] = "xxxxxxx";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"CL_GetCDKeyHash\".");

	hook_function(p, CL_GetCDKeyHash_hook, 9, &CL_GetCDKeyHash_hook_function_info);

	original_CL_GetCDKeyHash = (char* (*)(void))CL_GetCDKeyHash_hook_function_info.original_function_address;
}

void hook_CL_SendConnectPacket_Steam_GSInitiateGameConnection() {
	const unsigned char pattern[] = "\xE8\x10\x42\x07\x00\x83\xC4\x1C\x8B\xF0\xF2";
	const char mask[] = "x????xxxxxx";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"Steam_GSInitiateGameConnection\" inside \"CL_SendConnectPacket\".");

	hook_call(p, CL_SendConnectPacket_Steam_GSInitiateGameConnection_hook, &CL_SendConnectPacket_Steam_GSInitiateGameConnection_hook_call_info);

	original_Steam_GSInitiateGameConnection = (int (*)(void*, int, uint64, uint32, uint16, qboolean))CL_SendConnectPacket_Steam_GSInitiateGameConnection_hook_call_info.function_address;
}

void hook_R_ForceCVars() {
	const unsigned char pattern[] = "\x55\x8B\xEC\x83\x7D\x08\x00\x0F";
	const char mask[] = "xxxxx?xx";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"R_ForceCVars\".");

	hook_function(p, R_ForceCVars_hook, 7, &R_ForceCVars_hook_function_info);

	original_R_ForceCVars = (void (*)(qboolean))R_ForceCVars_hook_function_info.original_function_address;
}

void hook_pfnVGUI2DrawCharacterAdd() {
	original_pfnVGUI2DrawCharacterAdd = gEngfuncs->pfnVGUI2DrawCharacterAdd;
	gEngfuncs->pfnVGUI2DrawCharacterAdd = pfnVGUI2DrawCharacterAdd_hook;
}

void hook_pAddEntity() {
	original_pAddEntity = cl_funcs->pAddEntity;
	cl_funcs->pAddEntity = pAddEntity_hook;
}

xcommand_t hook_command_function(const char* const command_name, xcommand_t command_function) {
	cmd_function_s* p = (cmd_function_s*)gEngfuncs->GetFirstCmdFunctionHandle();

	xcommand_t ret;

	while (p != nullptr) {
		if (stricmp(p->name, command_name) == 0) {
			ret = p->function;

			p->function = command_function;

			return ret;
		}

		p = p->next;
	}

	return nullptr;
}

void hook_Host_Frame__Host_Frame() {
	const unsigned char pattern[] = "\xE8\xB2\x1A\x00\x00\xF3\x0F\x10\x05\x64\xB6\x31\x10\x83\xC4\x04\x0F\x57\xC9\x0F\x2E\xC1\x9F\xF6\xC4\x44\x7B\x08\xE8";
	const char mask[] = "x????xxxx????xxxxxxxxxxxxxx?x";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to find \"_Host_Frame\" inside \"Host_Frame\".");

	hook_call(p, Host_Frame__Host_Frame_hook, &Host_Frame__Host_Frame_hook_call_info);

	original__Host_Frame = (void (*)(float time))Host_Frame__Host_Frame_hook_call_info.function_address;
}

void patch_CL_Move() {
	const unsigned char pattern[] = "\x76\x31\x68\xF0\x6D\x2B\x10\x68\xA8\x5D\x2B\x10\xE8\x2E\xA4\x01\x00\xF2\x0F\x10\x05\xB8\x65\x30\x10\xF2";
	const char mask[] = "x?x????x????x????xxxx????x";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to apply first patch inside \"CL_Move\".");

	write_byte(p, 0xEB);

	p = (void*)((DWORD_PTR)p + 58);

	write_byte(p, 0xEB);

	const unsigned char pattern2[] = "\x7D\x09\x8B\xF1\xB8";
	const char mask2[] = "x?xxx";

	p = find_pattern(hw_module, pattern2, ARRAYSIZE(pattern2) - 1, mask2);

	if (p == nullptr)
		return Error("Failed to apply second patch inside \"CL_Move\".");

	write_byte(p, 0xEB);

	p = (void*)((DWORD_PTR)p + 16);

	write_byte(p, 0xEB);

	const unsigned char pattern3[] = "\x76\x1A\x68\xBC\x6E\x2B\x10\x68\x00\x5D\x2B\x10\xE8\x26\xA1\x01\x00\xF3\x0F\x10\x0D\x8C\xA0\x31\x10\x83\xC4\x08\x8B";
	const char mask3[] = "x?x????x????x????xxxx????xxxx";

	p = find_pattern(hw_module, pattern3, ARRAYSIZE(pattern3) - 1, mask3);

	if (p == nullptr)
		return Error("Failed to apply third patch inside \"CL_Move\".");

	write_byte(p, 0xEB);

	const unsigned char pattern4[] = "\x76\x11\xF3\x0F\x5E\xD9";
	const char mask4[] = "x?xxxx";

	p = find_pattern(hw_module, pattern4, ARRAYSIZE(pattern4) - 1, mask4);

	if (p == nullptr)
		return Error("Failed to apply fourth patch inside \"CL_Move\".");

	write_byte(p, 0x74);
}

void restore_CL_Move() {
	const unsigned char pattern[] = "\xEB\x31\x68\xF0\x6D\x2B\x10\x68\xA8\x5D\x2B\x10\xE8\x2E\xA4\x01\x00\xF2\x0F\x10\x05\xB8\x65\x30\x10\xF2";
	const char mask[] = "x?x????x????x????xxxx????x";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	write_byte(p, 0x76);

	p = (void*)((DWORD_PTR)p + 58);

	write_byte(p, 0x76);

	const unsigned char pattern2[] = "\xEB\x09\x8B\xF1\xB8";
	const char mask2[] = "x?xxx";

	p = find_pattern(hw_module, pattern2, ARRAYSIZE(pattern2) - 1, mask2);

	write_byte(p, 0x7D);

	p = (void*)((DWORD_PTR)p + 16);

	write_byte(p, 0x7E);

	const unsigned char pattern3[] = "\xEB\x1A\x68\xBC\x6E\x2B\x10\x68\x00\x5D\x2B\x10\xE8\x26\xA1\x01\x00\xF3\x0F\x10\x0D\x8C\xA0\x31\x10\x83\xC4\x08\x8B";
	const char mask3[] = "x?x????x????x????xxxx????xxxx";

	p = find_pattern(hw_module, pattern3, ARRAYSIZE(pattern3) - 1, mask3);

	write_byte(p, 0x76);

	const unsigned char pattern4[] = "\x74\x11\xF3\x0F\x5E\xD9";
	const char mask4[] = "x?xxxx";

	p = find_pattern(hw_module, pattern4, ARRAYSIZE(pattern4) - 1, mask4);

	write_byte(p, 0x76);
}

void patch_Cvar_CommandWithPrivilegeCheck() {
	const unsigned char pattern[] = "\x7E\x11\xFF\x37\x68";
	const char mask[] = "x?xxx";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to apply patch inside \"Cvar_CommandWithPrivilegeCheck\".");

	write_byte(p, 0xEB);
}

void restore_Cvar_CommandWithPrivilegeCheck() {
	const unsigned char pattern[] = "\xEB\x11\xFF\x37\x68";
	const char mask[] = "x?xxx";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	write_byte(p, 0x7E);
}

void patch_Info_SetValueForKey() {
	const unsigned char pattern[] = "\x75\x0F\x68\x2C\xEE\x2B\x10\xE8\x2B\x6E\xFE\xFF\x83\xC4\x04\x5D\xC3\x89";
	const char mask[] = "x?x????x????xxxxxx";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	if (p == nullptr)
		return Error("Failed to apply patch inside \"Info_SetValueForKey\".");

	write_byte(p, 0xEB);
}

void restore_Info_SetValueForKey() {
	const unsigned char pattern[] = "\xEB\x0F\x68\x2C\xEE\x2B\x10\xE8\x2B\x6E\xFE\xFF\x83\xC4\x04\x5D\xC3\x89";
	const char mask[] = "x?x????x????xxxxxx";

	void* p = find_pattern(hw_module, pattern, ARRAYSIZE(pattern) - 1, mask);

	write_byte(p, 0x75);
}

void memory_search_init() {
	load_modules();

	search_cl_funcs();
	search_gEngfuncs();
	search_cl_parsefuncs();
	search_msg_readcount();
	search_MSG_ReadString();
	search_MSG_ReadByte();
	search_Cmd_TokenizeString();
	search_cls();
	search_CL_RecordHUDCommand();
	search_COM_Parse();
	search_com_token();
	search_nMax();
	search_Cvar_DirectSet();
	search_Cmd_ForwardToServer();
	search_MSG_BeginReading();
	search_MSG_ReadLong();
	search_MSG_ReadStringLine();
	search_Cbuf_AddText();
	search_CL_Disconnect();
	search_MSG_WriteByte();
	search_MSG_WriteLong();
	search_MSG_WriteString();
	search_gClientUserMsgs();
	search_MSG_ReadShort();
	search_MSG_ReadCoord();
	search_MSG_ReadAngle();
	search_cmd_args();

	hook_SwapBuffers();
	original_CL_Parse_StuffText = (void (*)(void))hook_svc_function("svc_stufftext", CL_Parse_StuffText_hook);
	original_CL_Parse_Director = (void (*)(void))hook_svc_function("svc_director", CL_Parse_Director_hook);
	hook_CL_ReadDemoMessage_OLD_ValidStuffText();
	hook_CL_ReadDemoMessage_OLD_Cbuf_AddFilteredText();
	hook_CL_ReadPackets_CL_ConnectionlessPacket();
	hook_CL_BatchResourceRequest_IsSafeFileToDownload();
	hook_Netchan_CopyFileFragments_IsSafeFileToDownload();
	original_CL_Parse_VoiceInit = (void (*)(void))hook_svc_function("svc_voiceinit", CL_Parse_VoiceInit_hook);
	original_CL_Send_CvarValue = (void (*)(void))hook_svc_function("svc_sendcvarvalue", CL_Send_CvarValue_hook);
	original_CL_Send_CvarValue2 = (void (*)(void))hook_svc_function("svc_sendcvarvalue2", CL_Send_CvarValue2_hook);
	hook_CL_SendConsistencyInfo_MD5_Hash_File();
	original___MsgFunc_MOTD = (int (*)(const char*, int, void*))hook_user_msg("MOTD", __MsgFunc_MOTD_hook);
	hook_CL_GetCDKeyHash();
	hook_CL_SendConnectPacket_Steam_GSInitiateGameConnection();
	hook_R_ForceCVars();
	original_CL_Set_ServerExtraInfo = (void (*)(void))hook_svc_function("svc_sendextrainfo", CL_Set_ServerExtraInfo_hook);
	original_CL_Parse_Disconnect = (void (*)(void))hook_svc_function("svc_disconnect", CL_Parse_Disconnect_hook);
	hook_pfnVGUI2DrawCharacterAdd();
	hook_pAddEntity();
	original_CL_Parse_TempEntity = (void (*)(void))hook_svc_function("svc_temp_entity", CL_Parse_TempEntity_hook);
	original_Host_Motd_Write_f = hook_command_function("motd_write", null_function);
	original_Host_WriteCustomConfig = hook_command_function("writecfg", null_function);
	original_SV_WriteId_f = hook_command_function("writeid", null_function);
	hook_Host_Frame__Host_Frame();
	original_Host_Say_f = hook_command_function("say", Host_Say_f_hook);
	original_Host_Say_Team_f = hook_command_function("say_team", Host_Say_Team_f_hook);

	patch_CL_Move();
	patch_Cvar_CommandWithPrivilegeCheck();
	patch_Info_SetValueForKey();

	HANDLE mutex_handle = OpenMutex(MUTEX_MODIFY_STATE, FALSE, "ValveHalfLifeLauncherMutex");

	if (mutex_handle != nullptr) {
		ReleaseMutex(mutex_handle);
		CloseHandle(mutex_handle);
	}
}

void memory_search_release() {
	unhook_function(&SwapBuffers_hook_function_info);
	clean_imgui();
	hook_svc_function("svc_stufftext", original_CL_Parse_StuffText);
	hook_svc_function("svc_director", original_CL_Parse_Director);
	hook_call(CL_ReadDemoMessage_OLD_ValidStuffText_hook_call_info.address, original_ValidStuffText, nullptr);
	hook_call(CL_ReadDemoMessage_OLD_Cbuf_AddFilteredText_hook_call_info.address, original_Cbuf_AddFilteredText, nullptr);
	hook_call(CL_ReadPackets_CL_ConnectionlessPacket_hook_call_info.address, original_CL_ConnectionlessPacket, nullptr);
	hook_call(CL_BatchResourceRequest_IsSafeFileToDownload_hook_call_info.address, original_IsSafeFileToDownload, nullptr);
	hook_call(Netchan_CopyFileFragments_IsSafeFileToDownload_hook_call_info.address, original_IsSafeFileToDownload, nullptr);
	hook_svc_function("svc_voiceinit", original_CL_Parse_VoiceInit);
	hook_svc_function("svc_sendcvarvalue", original_CL_Send_CvarValue);
	hook_svc_function("svc_sendcvarvalue2", original_CL_Send_CvarValue2);
	hook_call(CL_SendConsistencyInfo_MD5_Hash_File_hook_call_info.address, original_MD5_Hash_File, nullptr);
	hook_user_msg("MOTD", original___MsgFunc_MOTD);
	unhook_function(&CL_GetCDKeyHash_hook_function_info);
	hook_call(CL_SendConnectPacket_Steam_GSInitiateGameConnection_hook_call_info.address, original_Steam_GSInitiateGameConnection, nullptr);
	unhook_function(&R_ForceCVars_hook_function_info);
	hook_svc_function("svc_sendextrainfo", original_CL_Set_ServerExtraInfo);
	hook_svc_function("svc_disconnect", original_CL_Parse_Disconnect);
	gEngfuncs->pfnVGUI2DrawCharacterAdd = original_pfnVGUI2DrawCharacterAdd;
	cl_funcs->pAddEntity = original_pAddEntity;
	hook_svc_function("svc_temp_entity", original_CL_Parse_TempEntity);
	hook_command_function("motd_write", original_Host_Motd_Write_f);
	hook_command_function("writecfg", original_Host_WriteCustomConfig);
	hook_command_function("writeid", original_SV_WriteId_f);
	hook_call(Host_Frame__Host_Frame_hook_call_info.address, original__Host_Frame, nullptr);
	hook_command_function("say", original_Host_Say_f);
	hook_command_function("say_team", original_Host_Say_Team_f);

	restore_CL_Move();
	restore_Cvar_CommandWithPrivilegeCheck();
}