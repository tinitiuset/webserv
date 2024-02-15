#ifndef CONF_HPP
#define CONF_HPP
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "Server.hpp"

class Conf {

private:
	std::string _file;
	std::vector<Server> _servers;

public:
	Conf(const std::string& file);
	~Conf();

	void parse();

	Server getServer(int index);

};

#endif