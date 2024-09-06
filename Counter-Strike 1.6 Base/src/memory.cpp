#include "memory.h"
#include <Windows.h>

bool load_module(const char* const module_name, Module& output) {
	HMODULE module_handle = GetModuleHandle(module_name);

	if (module_handle == NULL)
		return false;

	output.beginning = module_handle;

	PIMAGE_DOS_HEADER image_dos_header = (PIMAGE_DOS_HEADER)module_handle;
	PIMAGE_NT_HEADERS image_nt_headers = (PIMAGE_NT_HEADERS)((DWORD_PTR)image_dos_header + image_dos_header->e_lfanew);

	output.end = (void*)((DWORD_PTR)output.beginning + image_nt_headers->OptionalHeader.SizeOfImage - 1);

	return true;
}

void* find_pattern(Module& module, const unsigned char* const pattern, unsigned long size, const char* const mask) {
	if (module.beginning == nullptr || module.end == nullptr)
		return nullptr;

	void* p = module.beginning;
	bool found;
	unsigned long i;

	while ((void*)((DWORD_PTR)p + size - 1) <= module.end) {
		found = true;

		for (i = 0; i < size; ++i) {
			if (mask[i] == '?')
				continue;

			if (*(unsigned char*)((DWORD_PTR)p + i) != pattern[i]) {
				found = false;

				break;
			}
		}

		if (found)
			return p;

		p = (void*)((DWORD_PTR)p + 1);
	}

	return nullptr;
}

void write_byte(void* address, unsigned char byte) {
	DWORD old_protect;
	VirtualProtect(address, 1, PAGE_READWRITE, &old_protect);

	*(unsigned char*)address = byte;

	VirtualProtect(address, 1, old_protect, &old_protect);
}

void hook_call(void* address, void* const function_address, HookCallInfo* output) {
	DWORD old_protect;
	VirtualProtect(address, 5, PAGE_READWRITE, &old_protect);

	*(unsigned char*)address = 0xE8;

	void* p = (void*)((DWORD_PTR)address + 1);

	if (output != nullptr) {
		output->address = address;
		output->function_address = Absolute(p);
	}

	*(void**)p = Relative(function_address, p);

	VirtualProtect(address, 5, old_protect, &old_protect);
}

void hook_jmp(void* address, void* const jmp_to_address, HookJmpInfo* output) {
	DWORD old_protect;
	VirtualProtect(address, 5, PAGE_READWRITE, &old_protect);

	*(unsigned char*)address = 0xE9;

	void* p = (void*)((DWORD_PTR)address + 1);

	if (output != nullptr) {
		output->address = address;
		output->jmp_to_address = Absolute(p);
	}

	*(void**)p = Relative(jmp_to_address, p);

	VirtualProtect(address, 5, old_protect, &old_protect);
}

void hook_function(void* const function_address, void* const new_function_address, unsigned long size_of_first_instructions, HookFunctionInfo* output) {
	void* original_function_address = VirtualAlloc(nullptr, size_of_first_instructions + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	memcpy(original_function_address, function_address, size_of_first_instructions);

	hook_jmp((void*)((DWORD_PTR)original_function_address + size_of_first_instructions), (void*)((DWORD_PTR)function_address + size_of_first_instructions), nullptr);

	DWORD old_protect;
	VirtualProtect(function_address, 5, PAGE_READWRITE, &old_protect);

	hook_jmp(function_address, new_function_address, nullptr);

	VirtualProtect(function_address, 5, old_protect, &old_protect);

	if (output != nullptr) {
		output->function_address = function_address;
		output->original_function_address = original_function_address;
	}
}

void unhook_function(HookFunctionInfo* hook_function_info) {
	DWORD old_protect;
	VirtualProtect(hook_function_info->function_address, 5, PAGE_READWRITE, &old_protect);

	memcpy(hook_function_info->function_address, hook_function_info->original_function_address, 5);

	VirtualProtect(hook_function_info->function_address, 5, old_protect, &old_protect);

	VirtualFree(hook_function_info->original_function_address, 0, MEM_RELEASE);
}