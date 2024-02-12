#include "Cgi.hpp"

int main()
{
	Cgi	test("POST", "cgi-bin/morse.py", "NUM=rfw 2");

	std::cout << test.initCgi() << std::endl;

	return (0);
}