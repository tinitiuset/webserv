#include "Cookie.hpp"

int	main()
{

	std::map<std::string, std::string> headers;

	headers["Host"] = "localhost:8050";
	headers["Cookie"] = "1=1756850";

	Cookie::generateNewCookie();

	std::string test;

	if (!Cookie::isValidCookie(headers))
		test = Cookie::getCookieResponse();



	std::cout << test << std::endl;

	std::cout << Cookie::getSesId() << std::endl;

	return (0);
}