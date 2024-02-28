#include "Utils.hpp"
#include <sstream>

std::string Utils::toString(const int &num)
{
    std::stringstream ss;
    ss << num;
    return (ss.str());
}

int Utils::toInt(const std::string &str)
{
    std::stringstream ss(str);
    int num;
    ss >> num;
    return (num);
}