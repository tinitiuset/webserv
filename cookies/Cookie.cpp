#include "Cookie.hpp"

int	Cookie::_sesId = 1;

std::map<std::string, std::string> 	Cookie::_sessionDB;
int									Cookie::_hash;
std::string							Cookie::_newKey;
std::string							Cookie::_newValue;

/* Cookie::Cookie(): _hash(0) 
{
	_sessionDB["145"] = "0123456";
	_sessionDB["1"] = "12349";
}; */

/* Cookie::Cookie() {
    // Inicializa _sessionDB aquí
    _sessionDB = std::map<std::string, std::string>();
} */

int	Cookie::generateHash()
{
	int	hashValue;
	
	std::string	op = std::to_string(_sesId * 4242);
	const char *str = op.c_str();

	while (*str)
        hashValue = (hashValue << 5) + *str++;

	return (hashValue);
}

void	Cookie::generateNewCookie()
{
	_newKey = Utils::toString(_sesId); 
	_newValue = Utils::toString(generateHash());

	_sessionDB[_newKey] = _newValue;
	_sesId++;
}

std::string	Cookie::getCookieHeader()
{
	generateNewCookie();
	std::string cookieHeader = "Set-Cookie: " + _newKey + "=" + _newValue + "\r\n";
	return (cookieHeader);
}

std::string	Cookie::getCookieResponse()
{
	std::string cookieResponse = "HTTP/1.1 200 OK\r\n";
	cookieResponse += "Content-Type: text/html\r\n";
	cookieResponse += getCookieHeader();
	cookieResponse += "\r\n";
	return (cookieResponse);
}



/* bool	Cookie::isValidCookie(const std::string &httpRequest)
{
	size_t cookieHeaderPos = httpRequest.find("Cookie: ");

        if (cookieHeaderPos != std::string::npos)
		{
            size_t endOfCookieHeader = httpRequest.find("\r\n", cookieHeaderPos);

            if (endOfCookieHeader != std::string::npos)
			{
                std::string cookieString = httpRequest.substr(cookieHeaderPos + 8, endOfCookieHeader - (cookieHeaderPos + 8));
				size_t equalPos = cookieString.find('=');
            	if (equalPos != std::string::npos)
				{
                	std::string key = cookieString.substr(0, equalPos);
               		std::string value = cookieString.substr(equalPos + 1);

					std::map<std::string, std::string>::iterator it = _sessionDB.find(key);
                    if (it != _sessionDB.end() && it->second == value)
					{
						std::cout << "Key: " << _newKey << " Value: " << _newValue << std::endl;
						return (true);                    
					}
				}
			}
		}
	return (false);
} */

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
				{
					std::cout << "Si se encuentra y el valor coincide - Key: " << _newKey << " Value: " << _newValue << std::endl;
					return (true);
				}
			}
		}
	}
	return (false);
}

int		Cookie::getSesId() {return(_sesId);}
