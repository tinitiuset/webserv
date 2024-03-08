#include "Cookie.hpp"
#include "../utils/Utils.hpp"

std::string	Cookie::getSetCookieValue()
{
	std::srand(static_cast<unsigned int>(std::time(0)));

    unsigned int rNumb = 0;
    for (int i = 0; i < 10; ++i) {
        rNumb = rNumb * 10 + (std::rand() % 10);
    }
    rNumb = rNumb % 1000000000;
    std::string value = Utils::toString(rNumb);

    return value;
}


void Cookie::removeNlCr(std::string &line) 
{
    size_t found;    
    while ((found = line.find('\n')) != std::string::npos) {
        line.erase(found, 1);
    }
    while ((found = line.find('\r')) != std::string::npos) {
        line.erase(found, 1);
    }
}

std::vector<std::string> Cookie::extractCookieValues(const std::string &input)
{
    std::vector<std::string> webServValues;
    std::istringstream iss(input);
    std::string token;

    while (std::getline(iss, token, ';'))
	{
        size_t found = token.find("webserv=");
        if (found != std::string::npos)
            webServValues.push_back(token.substr(found + 8));
    }

    return (webServValues);
}

bool Cookie::isValidCookie(const std::map<std::string, std::string>& headers)
{
	std::vector<std::string> fileCookVals;
	std::vector<std::string> headCookVals;

	std::ifstream file("./cookies/cookies.txt");
	if (!file.is_open())
		return (false);

	std::string line;
	while (std::getline(file, line))
	{
		removeNlCr(line);
		fileCookVals.push_back(line);
	}
	file.close();
	
	std::map<std::string, std::string>::const_iterator it = headers.find("Cookie");

    if (it != headers.end())
	{
		headCookVals = extractCookieValues(it->second);
		//compare each headCookVals with all fileCookVals
		for (size_t i = 0; i < headCookVals.size(); i++)
			for (size_t j = 0; j < fileCookVals.size(); j++)
				if (headCookVals[i] == fileCookVals[j])
					return (true);
	}
	return (false);
}
