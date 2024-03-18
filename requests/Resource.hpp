#ifndef RESOURCE_HPP
# define RESOURCE_HPP

# include <string>
# include <iostream>
# include <fstream>
# include <dirent.h>
# include <map>
# include <signal.h>
# include <ctime>
# include <sys/wait.h>

# include "../utils/defaults.hpp"
# include "../utils/Logger.hpp"
# include "../utils/Utils.hpp"
# include "../utils/Codes.hpp"
# include "../exceptions/RequestException.hpp"
# include "../dynamicpages/ErrorPage.hpp"

class Resource {

public:
	Resource(std::string path, std::string method);
	Resource(const Resource&);
	Resource& operator=(const Resource&);
	~Resource();

	std::string load();
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
	std::string		buildCGI(std::string qStr);
	std::string    	initCgi(std::string interpret, std::string qStr);

private:
	std::string _path;
	int _status;
	std::string _method;
	char**		_env;

};

#endif