#include <iostream>
#include <fstream>
#include <sstream>
#include <fstream>
#include <vector>
#include "ConfigFile.hpp"
#include "Multiplexer.hpp"
#include "Server.hpp"
#include "defaults.hpp"

ConfigFile  *confG;

int main(int ac, char **av)
{
	std::string defPath = "configFiles/default.yml";
    
	if (ac > 2)
        return (std::cout << "Unexpected argument number" << std::endl, 1);
	else if (ac == 1)
		av[1] = (char *)defPath.c_str();

    std::ifstream   file(av[1]);
    if (!file.is_open())
		return (std::cerr << "Unexpected config file" << std::endl, 0);

    std::string     line;
    int             count = 0;
    while (std::getline(file, line)) 
        if (line.find("server:") != std::string::npos)
            ++count;
    if (count < 1)
        return (std::cerr << "Invalid config file" << std::endl, 0);
    file.close();

    ConfigFile confFile(av[1], count);

    confG = &confFile;

    std::cout << confG->_serverArr[0].getServerName() << std::endl;

    return (0);
}

