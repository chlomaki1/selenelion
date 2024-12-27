#include <map>
#include <string>

class Data {
private:
	Data();

	std::map<std::string, std::string> m_dungeonMap;
public:
	Data(Data const&) = delete;
	void operator=(Data const&)  = delete;
    static Data& instance();

	const char* getDungeonImage(const wchar_t *name);
};