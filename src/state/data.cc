#include "data.h"

#include "../util.h"

Data & Data::instance() {
	static Data instance;

	return instance;
}

Data::Data() {
	/// MONATERRA //
}

const char *Data::getDungeonImage(const wchar_t* name) {
	auto it = m_dungeonMap.find(std::string(util::wcstrtocstr(name)));

	if (it != m_dungeonMap.end()) {
		return it->second.c_str();
	}

	return "default"; // or some default value
}
