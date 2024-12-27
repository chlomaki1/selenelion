
#include "../util.h"
#include "character.h"

Character::Character() :
	m_level(0),
	m_name((const wchar_t*)L"")
{

}

Character::Character(int level, const wchar_t* name) :
	m_level(level),
	m_name(name)
{

}

int Character::getLevel() {
	return m_level;
}

const wchar_t* Character::getName() {
	return m_name;
}

void Character::setLevel(int level) {
	m_level = level;
}

void Character::setName(const wchar_t* name) {
	m_name = name;
}