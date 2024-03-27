#include "Logger.hpp"

void Logger::trace(const std::string& msg) {
	if (false)
		std::cout << "\033[1;35m[TRACE] " << msg << "\033[0m" << std::endl;
}

void Logger::debug(const std::string& msg) {
	if (false)
		std::cout << "\033[1;34m[DEBUG] " << msg << "\033[0m" << std::endl;
}

void Logger::info(const std::string& msg) {
	std::cout << "[INFO] " << msg << std::endl;
}

void Logger::warning(const std::string& msg) {
	std::cout << "\033[1;33m[WARNING] " << msg << "\033[0m" << std::endl;
}

void Logger::error(const std::string& msg) {
	std::cerr << "\033[1;31m[ERROR] " << msg << "\033[0m" << std::endl;
}