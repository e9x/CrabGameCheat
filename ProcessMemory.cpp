#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "./ProcessMemory.h"
#include <TlHelp32.h>

ProcessMemory::ProcessMemory(const wchar_t* n) : name(n) {}

int ProcessMemory::write(uintptr_t address, size_t size, const void* value) {
	size_t bytes_read = 0;
	if (WriteProcessMemory(handle, (void*)address, value, size, &bytes_read)) return ProcessStatus::Ok;
	else return ProcessStatus::Unknown;
}

int ProcessMemory::read(uintptr_t address, size_t size, void* value) {
	size_t bytes_read = 0; 
	if (ReadProcessMemory(handle, (void*)address, value, size, &bytes_read)) return ProcessStatus::Ok;
	else return ProcessStatus::Unknown;
}

ProcessMemory::~ProcessMemory() {
	close();
}

int ProcessMemory::close() {
	if (!handle || !CloseHandle(handle)) return ProcessStatus::HandleClose;

	pid = NULL;
	handle = NULL;
	return ProcessStatus::Ok;
}

int ProcessMemory::open() {
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(entry);

	do {
		if (!lstrcmpi(entry.szExeFile, name)) {
			CloseHandle(snapshot);
			pid = entry.th32ProcessID;
			handle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
			return ProcessStatus::Ok;
		}
	} while (Process32Next(snapshot, &entry));
	
	CloseHandle(snapshot);
	return ProcessStatus::NotFound;
}