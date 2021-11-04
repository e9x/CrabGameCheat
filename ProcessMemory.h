#pragma once
#include <windows.h>

namespace ProcessStatus {
	constexpr int Ok = 0,
		NotFound = 1,
		InvalidSnapshot = 2,
		HandleClose = 3,
		Unknown = 4;
};

class ProcessMemory {
public:
	HANDLE handle = 0;
	DWORD pid = 0;
	const wchar_t* name = 0;

	ProcessMemory(const wchar_t* name);
	~ProcessMemory();

	int write(uintptr_t address, size_t size, const void* value);
	int read(uintptr_t address, size_t size, void* value);

	template<class var>
	inline int write(uintptr_t address, const var& value) {
		return write(address, sizeof(var), (void*)&value);
	}

	template<class var>
	inline int read(uintptr_t address, var& value) {
		return read(address, sizeof(var), &value);
	}

	int open();
	int close();
};