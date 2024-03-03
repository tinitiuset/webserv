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
	//create a random number of length 10 with seed time

    unsigned int rNumb = 0;
    for (int i = 0; i < 10; ++i) {
        rNumb = rNumb * 10 + (std::rand() % 10);
    }
    rNumb = rNumb % 1000000000;
	std::string value = Utils::toString(rNumb);
	return (value);
}


bool Cookie::isValidCookie(const std::map<std::string, std::string>& headers)
{
	for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
	{
		if (it->first == "Cookie")
		{
			
		}
	}
	std::cout << "Cookie is not valid" << std::endl;
	return (false);
}

int		Cookie::getSesId() {return(_sesId);}
