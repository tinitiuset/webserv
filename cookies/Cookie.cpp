#include "Cookie.hpp"

Cookie::Cookie(): _sesId(1), _hash(0) 
{
	_sessionDB["145"] = "0123456";
	_sessionDB["1"] = "12349";
};




int	Cookie::generateHash(const int &id)
{
	int	hashValue;
	
	const char *str = std::to_string(id * 4242).c_str();

	while (*str)
        hashValue = (hashValue << 5) + *str++;

	return (hashValue);
}

bool	Cookie::isValidCookie(const std::string &httpRequest)
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
						return (true);                    
				}

			}
		}
	return (false);
}