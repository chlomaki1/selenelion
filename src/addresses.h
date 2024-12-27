#pragma once

#include <iostream>
#include <ostream>
#include <Windows.h>
#include <tlhelp32.h>
#include <vector>

#include <Windows.h>
#include <cstdint>

namespace memory {
  template <typename T>
bool SafeReadMemory(uintptr_t address, T& outValue)
  {
    __try
    {
      if (address == NULL) {
        return false;
      }

      outValue = *reinterpret_cast<T*>(address);
      return true;
    }
    __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION
              ? EXCEPTION_EXECUTE_HANDLER
              : EXCEPTION_CONTINUE_SEARCH)
    {
      // If an access violation occurs, we handle it here
      return false;
    }
  }

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
    uintptr_t baseAddr = GetModuleBaseAddress(L"x2.exe");
    if (!baseAddr)
      return NULL;

    uintptr_t addr = baseAddr + initialOffset;

    for (int off : offsets)
    {
      if (addr == 0)
        return NULL;

      uintptr_t temp = 0;
      if (!SafeReadMemory(addr, temp))
      {
        return NULL;
      }

      addr = temp + off;
    }

    return addr;
  }

  template <typename T>
  T ReadMemory(uintptr_t address, T defaultValue = T{})
  {
    T value{};

    if (SafeReadMemory(address, value))
    {
      return value;
    }

    return defaultValue;
  }
}