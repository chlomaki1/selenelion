#pragma once

#include <cstdint>
#include <functional>

namespace util {
	const char* wcstrtocstr(const wchar_t* wideString);
	int64_t getCurrentTimestamp();
	void wstrclone(const wchar_t* src, const std::function<void(const wchar_t*)>& assignFunc);
} // namespace util