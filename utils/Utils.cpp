#include "Utils.hpp"
#include <sstream>

std::string Utils::toString(const int &num)
{
    std::stringstream ss;
    ss << num;
    return (ss.str());
}
