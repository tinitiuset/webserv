#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <string>

/*
 * Usage:
 * Logger::trace("This is a trace message");
 * Logger::debug("This is a debug message");
 * Logger::info("This is an info message");
 * Logger::warning("This is a warning message");
 * Logger::error("This is an error message");
 */
class Logger {
public:
	static void trace(const std::string&);
	static void debug(const std::string&);
	static void info(const std::string&);
	static void warning(const std::string&);
	static void error(const std::string&);
};

#endif