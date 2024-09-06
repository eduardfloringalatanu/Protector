#pragma once

struct Module {
	void* beginning;
	void* end;
};

bool load_module(const char* const module_name, Module& output);
void* find_pattern(Module& module, const unsigned char* const pattern, unsigned long size, const char* const mask);

void write_byte(void* address, unsigned char byte);

#define Absolute(p) (void*)((DWORD_PTR)p + *(DWORD_PTR*)p + sizeof(void*))
#define Relative(p, q) (void*)((DWORD_PTR)p - (DWORD_PTR)q - sizeof(void*))

struct HookCallInfo {
	void* address;
	void* function_address;
};

void hook_call(void* address, void* const function_address, HookCallInfo* output);

struct HookJmpInfo {
	void* address;
	void* jmp_to_address;
};

void hook_jmp(void* address, void* const jmp_to_address, HookJmpInfo* output);

struct HookFunctionInfo {
	void* function_address;
	void* original_function_address;
};

void hook_function(void* const function_address, void* const new_function_address, unsigned long size_of_first_instructions, HookFunctionInfo* output);
void unhook_function(HookFunctionInfo* hook_function_info);