#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <ctime>
#include <signal.h>

#include "../utils/defaults.hpp"
#include "../utils/Logger.hpp"
#include "../utils/Utils.hpp"

class Resource {

public:
	Resource(std::string path, std::string method);
	Resource(const Resource&);
	Resource& operator=(const Resource&);
	~Resource();

	std::string load() const;
	std::string status() const;
	std::string mime() const;

	//autoindex
	std::string 	buildAI(std::string uri, std::string host, std::string resPath);
    std::string     getPreviousUri(std::string uri);

	//cgi
	std::string 	extractCgi();
	std::string 	extractQStr();
	std::string     readChildOutput(int fd_child_to_parent);
	void            set4GETEnv(std::string cgiPath, std::string qStr);
    void   		    set4Post();
	std::string		buildCGI();
	std::string    	initCgi(std::string cgiPath, std::string interpret, std::string qStr);

private:
	std::string _path;
	std::string _status;
	std::string _method;
	char**		_env;

	//cgi

};

#endif