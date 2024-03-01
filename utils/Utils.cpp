#include "Utils.hpp"
#include <sstream>

std::string Utils::toString(const int &num)
{
    std::stringstream ss;
    ss << num;
    return (ss.str());
}

//deletes strFind in strFind
std::string Utils::strErase(std::string strBig, std::string strFind)
{
	size_t pos = strBig.find(strFind);

	if (pos != std::string::npos)
	{
		strBig.erase(pos, strFind.length());
		return (strBig);
	}
	else
		return ("");
}

//In a string (strBig) finds strFind and replaces it by strReplace
std::string	Utils::strReplace(std::string strBig, std::string strFind, std::string strReplace)
{
	size_t pos = strBig.find(strFind);

	if (pos != std::string::npos)
		return (strBig.substr(0, pos) + strReplace + strBig.substr(pos + strFind.length()));
	else
		return ("");
}

/* 
	receives a string vector and a string
	search str in the string vector
	returns:
	* the index where its found
	* -1 if not found
*/
int Utils::findStrInVector(const std::vector<std::string> &vec, const std::string &target)
{
	std::vector<std::string>::const_iterator it = std::find(vec.begin(), vec.end(), target);

	if (it != vec.end())
		return (std::distance(vec.begin(), it));
	else
		return (-1);
}

// checks if the path is a valid file
bool Utils::isFile(const char* path)
{
    struct stat info;
    if (stat(path, &info) != 0)
        return (false);

    return S_ISREG(info.st_mode);
}

bool Utils::isDirectory(const char* path)
{
    struct stat info;
    if (stat(path, &info) != 0)
        return false;

    return S_ISDIR(info.st_mode);
}

/* 
	Removes last "/" in a string if the string is anything different than "/"
*/
std::string  Utils::removeLastSlash(std::string str)
{
	if (str.length() > 1 && str[str.length() - 1] == '/')
	{
        str.erase(str.length() - 1);
    }
	return (str);
}
int Utils::toInt(const std::string &str)
{
    std::stringstream ss(str);
    int num;
    ss >> num;
    return (num);
}
