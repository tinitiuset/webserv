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
  	{
		std::cerr << "Usage: " << argv[0] << " [config_file]" << std::endl;
		return 1;
	}
	
	if (!conf->parse())
		return (delete conf, 1);

	conf->load();

	Multiplexer().run();

	delete conf;
	return 0;
}