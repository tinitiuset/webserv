#include "Cookie.hpp"


int	main()
{
	Cookie inst;

	for (int i = 0; i < 2; i++)
	{
		std::string httpRequest = "GET /ruta HTTP/1.1\r\nHost: ejemplo.com\r\nCookie: 1=1756850\r\n\r\n";

		std::cout << "session id: " << inst.getSesId() << std::endl;

		if (!inst.isValidCookie(httpRequest))
		{
			std::cout << "is not valid. We have to generate new cookie, id++ and set" << std::endl;
			inst.generateNewCookie();
			std::cout << inst.getCookieResponse() << std::endl;
		}
		else
			std::cout << "is valid cookie. We do nothing" << std::endl;	
	}
}