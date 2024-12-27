
#include "../util.h"
#include "location.h"

Location::Location(const wchar_t* name) : m_name(name) {}

const wchar_t* Location::getName() {
	return m_name;
}

void Location::setName(const wchar_t* name) {
	m_name = name;
}