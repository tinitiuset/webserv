#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sys/stat.h>
#include <map>

class Utils
{
    public:
        static std::string  toString(const int &num);
        static int          toInt(const std::string &str);
        static std::string  strErase(std::string strBig, std::string strFind);
        static std::string  strReplace(std::string strBig, std::string strFind, std::string strReplace);
        static std::string  removeLastSlash(std::string str);
        static  int         findStrInVector(const std::vector<std::string> &vec, const std::string &target);
        static  bool        isDirectory(const char* path);
        static  bool        isFile(const char* path);
        static int	        ft_strcmp(const char *s1, const char *s2);
        static std::string  extractFilePath(const std::string uri);
        static std::string  extractQStr(const std::string uri);
};

#endif
