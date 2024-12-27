#pragma once

class Character {
private:
  int m_level;
  const wchar_t* m_name;

public:
  Character();
  Character(int level, const wchar_t* name);

  int getLevel();
  const wchar_t* getName();
  void setLevel(int level);
  void setName(const wchar_t* name);
};