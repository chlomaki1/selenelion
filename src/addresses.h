#pragma once

#include <iostream>
#include <ostream>
#include <Windows.h>
#include <tlhelp32.h>
#include <vector>

uintptr_t GetModuleBaseAddress(const wchar_t* moduleName)
{
	uintptr_t baseAddress = 0;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());

	if (snapshot != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32W moduleEntry;
		moduleEntry.dwSize = sizeof(MODULEENTRY32W);

		if (Module32FirstW(snapshot, &moduleEntry))
		{
			do
			{
				if (!_wcsicmp(moduleEntry.szModule, moduleName))
				{
					baseAddress = reinterpret_cast<uintptr_t>(moduleEntry.modBaseAddr);
					break;
				}
			} while (Module32NextW(snapshot, &moduleEntry));
		}

		CloseHandle(snapshot);
	}
	return baseAddress;
}

uintptr_t CalculateAddress(
	int initialOffset,
	const std::vector<int>& offsets)
{
	uintptr_t addr = GetModuleBaseAddress(L"x2.exe") + initialOffset;
	for (int off : offsets)
	{
		if (addr == NULL) {
			return NULL;
		}

		addr = *reinterpret_cast<uintptr_t*>(addr);  // dereference
		addr += off;
	}
	return addr;
}