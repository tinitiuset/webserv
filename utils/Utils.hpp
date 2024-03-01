#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sys/stat.h>

class Utils
{
    public:
        static std::string  toString(const int &num);
        static std::string  strErase(std::string strBig, std::string strFind);
        static std::string  strReplace(std::string strBig, std::string strFind, std::string strReplace);
        static std::string  removeLastSlash(std::string str);
        static  int         findStrInVector(const std::vector<std::string> &vec, const std::string &target);
        static  bool        isDirectory(const char* path);
        static  bool        isFile(const char* path);
};

#endif
