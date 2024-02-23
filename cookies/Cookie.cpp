#include "Cookie.hpp"
#include "../utils/Utils.hpp"

int	Cookie::_sesId = 1;

std::map<std::string, std::string> 	Cookie::_sessionDB = {};

/* Cookie::Cookie(): _hash(0) 
{
	_sessionDB["145"] = "0123456";
	_sessionDB["1"] = "12349";
}; */

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
	std::string cookieResponse = "Set-Cookie: " + _newKey + "=" + _newValue + "\r\n";
	return (cookieResponse);
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
        // Iterar sobre el mapa de encabezados para buscar la clave "Cookie"
        for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
            if (it->first == "Cookie") { // Si se encuentra la clave "Cookie"
                // Obtener el valor asociado a la clave "Cookie"
                std::string cookieString = it->second;

                // Buscar el signo de igual '=' en la cadena del cookie
                size_t equalPos = cookieString.find('=');
                if (equalPos != std::string::npos) { // Si se encuentra el signo de igual
                    // Extraer la clave y el valor del cookie
                    std::string key = cookieString.substr(0, equalPos);
                    std::string value = cookieString.substr(equalPos + 1);

                    // Buscar el par clave-valor en la base de datos de sesiones
                    std::map<std::string, std::string>::iterator dbIt = _sessionDB.find(key);
                    if (dbIt != _sessionDB.end() && dbIt->second == value) { // Si se encuentra y el valor coincide
                        std::cout << "Key: " << _newKey << " Value: " << _newValue << std::endl;
                        return true;
                    }
                }
            }
        }
        return false; // Si no se encuentra la clave "Cookie" o no se encuentra la sesión en la base de datos
    }

int		Cookie::getSesId() {return(_sesId);}
