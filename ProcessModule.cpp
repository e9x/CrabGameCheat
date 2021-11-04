#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "./ProcessModule.h"
#include <TlHelp32.h>

ProcessModule::operator uintptr_t() {
	return address;
}

ProcessModule::ProcessModule(const wchar_t* n) : name(n) {}

int ProcessModule::open(ProcessMemory& memory) {
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, memory.pid);
	if (snapshot == INVALID_HANDLE_VALUE) return ProcessStatus::InvalidSnapshot;

	MODULEENTRY32 entry;
	entry.dwSize = sizeof(entry);

	do {
		if (!lstrcmpi(entry.szModule, name)) {
			CloseHandle(snapshot);
			address = (uintptr_t)entry.modBaseAddr;
			size = entry.modBaseSize;
			h = entry.hModule;
			return ProcessStatus::Ok;
		}
	} while (Module32Next(snapshot, &entry));

	CloseHandle(snapshot);
	return ProcessStatus::NotFound;
}