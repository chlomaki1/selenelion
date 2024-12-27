#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

enum class Level {
	DEBUG,
	INFO,
	WARNING,
	ERR
};

class Logger {
	public:
		void log(Level level, const std::string& title, const std::string& message) {
			std::string levelStr;
			std::string colorCode;

			switch (level) {
				case Level::DEBUG:
					levelStr = "DEBUG";
				colorCode = "\033[97m"; // White
				break;
				case Level::INFO:
					levelStr = "INFO";
				colorCode = "\033[38;5;48m"; // Approximation for #40ff63
				break;
				case Level::WARNING:
					levelStr = "WARNING";
				colorCode = "\033[38;5;227m"; // Approximation for #ffef40
				break;
				case Level::ERR:
					levelStr = "ERROR";
				colorCode = "\033[38;5;203m"; // Approximation for #ff4040
				break;
			}

			std::string timestamp = getCurrentTimestamp();
			std::cout << colorCode << timestamp << " | " << levelStr << " | (" << title << ") >> " << message << "\033[0m" << std::endl;
		}

	private:
		std::string getCurrentTimestamp() {
			auto now = std::chrono::system_clock::now();
			auto in_time_t = std::chrono::system_clock::to_time_t(now);
			std::stringstream ss;
			ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M");
			return ss.str();
		}
};