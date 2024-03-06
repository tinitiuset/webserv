#include "Cgi.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include "defaults.hpp"
#include "Utils.hpp"
#include <ctime>
#include <signal.h>

//CHECK ERROR MANAGEMENT


Cgi::Cgi(std::string method, std::string uri, std::string qustr):
_method(method), _uri(uri), _quStr(qustr), _env(NULL)
{
    if (uri.substr(uri.length() - 3) == ".py")
	{
		_cgi = "python3";
        _interpret = "/usr/local/bin/python3";
	}
    else if (uri.substr(uri.length() - 3) == ".pl")
	{
		_cgi = "perl";
        _interpret = "/usr/bin/perl";
	}
    else
        throw std::runtime_error("invalid cgi script");
}




Cgi::~Cgi()
{
    delete _env;
}

std::string    Cgi::initCgi()
{
	int	fd_parent_to_child[2];
    int fd_child_to_parent[2];

	if (pipe(fd_child_to_parent) == -1)
        throw std::runtime_error("pipe error");
    
    if (_method == "GET")
        set4GETEnv();
    else if (_method == "POST")
    {
		if (pipe(fd_parent_to_child) == -1)
			throw std::runtime_error("pipe error");
		set4Post();
	}
    else
    {
        std::cerr << "method not supported" << std::endl;
        return (""); //?
    }

    pid_t pid = fork();

    if (pid == -1)
        throw std::runtime_error("fork error");
    else if (pid == 0)
    {
        if (_method == "POST")
		{
			close(fd_parent_to_child[1]);
			dup2(fd_parent_to_child[0], STDIN_FILENO);   
		}
		close(fd_child_to_parent[0]);
		dup2(fd_child_to_parent[1], STDOUT_FILENO);
		close(fd_child_to_parent[1]);
		if (_method == "POST")
			close(fd_parent_to_child[0]);

        char *args[3];
		args[0] = (char *)_cgi.c_str();
        args[1] = (char *)_uri.c_str();
        args[2] = NULL;

        if (execve(_interpret.c_str(), args, _env) == -1)
            throw std::runtime_error("execve error");    
    }
	//parent process
	if (_method == "POST")
	{
		write(fd_parent_to_child[1], _quStr.c_str(), _quStr.length());
		close(fd_parent_to_child[0]);
		close(fd_parent_to_child[1]);
	}
	close(fd_child_to_parent[1]);
	
	std::time_t start_time = std::time(NULL);
	int status;
	while (waitpid(pid, &status, WNOHANG) == 0)
	{
		std::time_t current_time = std::time(NULL);
		if (current_time - start_time > TIMEOUT)
		{
			std::cout << "Timeout. Killing child process" << std::endl;
			kill(pid, SIGKILL);
			return ("");
		}
		usleep(100000);  // (0.1 sec)
	}

	return (readChildOutput(fd_child_to_parent[0]));
}

void    Cgi::set4GETEnv()
{
    _env = new char*[7];

    std::vector<std::string>    envVect;

    envVect.push_back("GATEWAY_INTERFACE=CGI/1.1");
    envVect.push_back("SERVER_PROTOCOL=HTTP/1.1");
    envVect.push_back("REQUEST_METHOD=" + _method);
    if (access(_uri.c_str(), X_OK))
        throw std::runtime_error("invalid script or not found");
    envVect.push_back("SCRIPT_NAME=" + _uri);
    envVect.push_back("QUERY_STRING=" + _quStr);
    envVect.push_back("CONTENT_LENGTH=1024");

    for (int i = 0; i < 7; i++)
        _env[i] = (char *)envVect[i].c_str();
    _env[7] = NULL;
}

void    Cgi::set4Post()
{
    _env = new char*[3];

    std::vector<std::string>    envVect;

    envVect.push_back("REQUEST_METHOD=" + _method);
    envVect.push_back("CONTENT_LENGTH=1024");

    for (int i = 0; i < 3; i++)
        _env[i] = (char *)envVect[i].c_str();
    _env[3] = NULL;
}

std::string     Cgi::readChildOutput(int fd_child_to_parent)
{
	char   buffer[9056];
	std::string resp;
	ssize_t bytes;
	while ((bytes = read(fd_child_to_parent, buffer, sizeof(buffer))) > 0)
	{
		if (bytes < 0)
			throw std::runtime_error("fd read error");
		buffer[bytes] = '\0';
		resp += buffer;
	}
	close(fd_child_to_parent);
	return(resp);
}
