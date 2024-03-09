#ifndef ERRORPAGE_HPP
#define ERRORPAGE_HPP

#include <string>
#include "../utils/Utils.hpp"

class ErrorPage {

public:
	static std::string build(int status, std::string message);
};

#endif
