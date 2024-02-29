#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <dirent.h>

#include "../utils/Logger.hpp"
#include "../utils/Utils.hpp"

class Resource {

public:
	Resource(std::string);
	Resource(const Resource&);
	Resource& operator=(const Resource&);
	~Resource();

	std::string load() const;

	std::string status() const;

	std::string 	buildAI(std::string uri, int port, std::string address, std::string resPath);
    std::string     getPreviousUri(std::string uri);


private:
	std::string _path;
	std::string _status;

};

#endif