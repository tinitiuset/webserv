#include "Multiplexer.hpp"
#include "utils/defaults.hpp"
#include <string>

Conf* conf;

int main(int argc, char **argv)
{
	if (argc == 1) 
		conf = new Conf("./config/default.conf");
  	else if (argc == 2)
		conf = new Conf(argv[1]);
  	else 
		return std::cerr << "Usage: " << argv[0] << " [config_file]" << std::endl, 1;

	if (!conf->parse())
		return (delete conf, 1);
	try {
		conf->load();
		conf->validate();
		Multiplexer().run();
	}
	catch (std::runtime_error &e) {
		Logger::error(e.what());
		return (delete conf, 1);
	}
	catch (std::exception &e) {
		Logger::error(e.what());
		return (delete conf, 1);
	}
	return (delete conf, 0);
}