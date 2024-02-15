#include <iostream>
#include "./model/Conf.hpp"

int main() {

	Conf conf("./config/webserv.conf");
	conf.parse();

	std::string name = conf.getServer(0).server_name();
	Redirect* redirectLocation = dynamic_cast<Redirect*>(conf.getServer(0).location("/redirect"));

	Server server(conf.getServer(0));

	dynamic_cast<Redirect*>(conf.getServer(0).location("/redirect")) == nullptr ?
		std::cout << "Not a redirect" << std::endl
		:
		std::cout << "Is a redirect" << std::endl;

	return 0;
}