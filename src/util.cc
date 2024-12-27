#include "util.h"

#include <chrono> // for std::chrono
#include <codecvt> // for std::wcsrtombs
#include <cwchar> // for wcslen, wmemcpy, etc.
#include <functional>
#include <iostream>

namespace util {

	const char* wcstrtocstr(const wchar_t* wideString) {
		std::size_t bufferSize = std::wcsrtombs(nullptr, &wideString, 0, nullptr) + 1; // +1 for null terminator
		char* buffer = new char[bufferSize];
		std::mbstate_t state = std::mbstate_t();
		std::wcsrtombs(buffer, &wideString, bufferSize, &state);

		return buffer;
	}

	int64_t getCurrentTimestamp() {
		auto now = std::chrono::system_clock::now();
		auto duration = now.time_since_epoch();
		auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

		return milliseconds.count();
	}

	void wstrclone(const wchar_t* src, const std::function<void(const wchar_t*)>& assignFunc)
	{
		if (!src)
		{
			// Nothing to clone
			return;
		}

		// Construct a std::wstring from src
		std::wstring cloned{ src };

		// Pass the wstring itself to the callback
		assignFunc(cloned.c_str());
	}
} // namespace util