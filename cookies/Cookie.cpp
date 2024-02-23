#include "Cookie.hpp"
#include "../utils/Utils.hpp"

int	Cookie::_sesId = 1;

std::map<std::string, std::string> 	Cookie::_sessionDB;
int									Cookie::_hash;
std::string							Cookie::_newKey;
std::string							Cookie::_newValue;

int	Cookie::generateHash()
{
	int	hashValue;
	
	std::string	op = Utils::toString(_sesId * 4242);
	const char *str = op.c_str();

	while (*str)
        hashValue = (hashValue << 5) + *str++;

	return (hashValue);
}

void	Cookie::generateNewCookie()
{
	std::cout << "Generating new cookie" << std::endl;
	
	_newKey = Utils::toString(_sesId); 
	_newValue = Utils::toString(generateHash());

	_sessionDB[_newKey] = _newValue;
	_sesId++;
}

std::string	Cookie::getSetCookieValue()
{
	generateNewCookie();
	std::string value = _newKey + "=" + _newValue + "\r\n";
	return (value);
}

std::string	Cookie::getCookieResponse()
{
	std::string cookieResponse = "HTTP/1.1 200 OK\r\n";
	cookieResponse += "Content-Type: text/html\r\n";
	cookieResponse += getSetCookieValue();
	cookieResponse += "\r\n";
	return (cookieResponse);
}

bool Cookie::isValidCookie(const std::map<std::string, std::string>& headers)
{
	for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
	{
		if (it->first == "Cookie")
		{
			std::string cookieString = it->second;
			size_t equalPos = cookieString.find('=');
			if (equalPos != std::string::npos)
			{
				std::string key = cookieString.substr(0, equalPos);
				std::string value = cookieString.substr(equalPos + 1);
				std::map<std::string, std::string>::iterator dbIt = _sessionDB.find(key);
				if (dbIt != _sessionDB.end() && dbIt->second == value)
					return (true);
			}
		}
	}
	return (false);
}

int		Cookie::getSesId() {return(_sesId);}
