#pragma once

class Location {
	private:
		const wchar_t* m_name;

	public:
		Location(const wchar_t* name);

		const wchar_t* getName();
		void setName(const wchar_t* name);
};