#pragma once

#include <Windows.h>

const int x2_base = reinterpret_cast<uintptr_t>(GetModuleHandle(0));

#define OFFSET(n) reinterpret_cast<uintptr_t>(GetModuleHandle(0) + x2)

#pragma region X2Data
#define X2DATA_CTOR 0x125C2F0
#pragma endregion