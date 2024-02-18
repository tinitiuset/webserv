#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <string>
#include <iostream>
#include <fstream>

#include "../utils/Logger.hpp"

class Resource {

public:
	Resource(std::string);
	Resource(const Resource&);
	Resource& operator=(const Resource&);
	~Resource();

	std::string load() const;

	std::string status() const;

private:
	std::string _path;
	std::string _status;

};

#endif