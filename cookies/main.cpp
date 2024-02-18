#include "Cookie.hpp"


int	main()
{
	std::string httpRequest = "GET /ruta HTTP/1.1\r\nHost: ejemplo.com\r\nCookie: 145=0123456\r\n\r\n";
	Cookie inst;

	std::cout << inst.isValidCookie(httpRequest) << std::endl;
}