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