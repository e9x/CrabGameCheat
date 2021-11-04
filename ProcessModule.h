#pragma once
#include "./ProcessMemory.h"

class ProcessModule {
public:
	const wchar_t* name;
	HMODULE h;
	size_t size;
	uintptr_t address;
	ProcessModule(const wchar_t* name);
	int open(ProcessMemory& memory);
	operator uintptr_t();
};