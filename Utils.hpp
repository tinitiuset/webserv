#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>

class Utils
{
    public:
        static std::string toString(const int &num);
        static std::string strErase(std::string strBig, std::string strFind);
        static std::string strReplace(std::string strBig, std::string strFind, std::string strReplace);
};

#endif
