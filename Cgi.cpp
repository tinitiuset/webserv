#include "Cgi.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include "defaults.hpp"
#include "Utils.hpp"
//#include <string>

//CHECK ERROR MANAGEMENT


Cgi::Cgi(std::string method, std::string uri, std::string qustr):
_method(method), _uri(uri), _quStr(qustr)
{
    if (uri.substr(uri.length() - 3) == ".py")
        _interpret = "/usr/local/bin/python3";
    else if (uri.substr(uri.length() - 3) == ".sh")
        _interpret = "/bin/bash";
    else
        throw std::runtime_error("invalid cgi script");
    _env = NULL;
}


Cgi::~Cgi()
{
    delete _env;
}

void    Cgi::selMethod()
{
    _fdOut = dup(STDOUT_FILENO);
    _fdIn = dup(STDIN_FILENO);
    if (pipe(_fd) == -1)
        throw std::runtime_error("pipe error");
    
    if (_method == "GET") //also check if method is supported in config????
        set4GETEnv();
    else if (_method == "POST") //also check if method is supported in config????
        set4Post();
    else
    {
        std::cerr << "method not supported" << std::endl;
        return ; //?
    }

    close(_fd[1]);

    pid_t pid = fork();

    if (pid == -1)
        throw std::runtime_error("fork error");
    else if (pid == 0)
    {
        close(_fd[0]);
        close(_fd[1]);

        char *args[2];
        args[0] = _uri.c_str();
        args[1] = NULL;

        if (execve(_interpret.c_str(), args, _env) == -1)
            throw std::runtime_error("execve error");
            
    }

    

}

void    Cgi::set4GETEnv()
{
    char    **env = new char*[7];

    std::vector<std::string>    envVect;

    envVect.push_back("GATEWAY_INTERFACE=CGI/1.1");
    envVect.push_back("SERVER_PROTOCOL=HTTP/1.1");
    envVect.push_back("REQUEST_METHOD=" + _method);
    
    //si llega como GET /cgi-bin/sos.py
    if (access(_uri.c_str(), X_OK))
        throw std::runtime_error("script not found");
    envVect.push_back("SCRIPT_NAME=" + _uri);
    envVect.push_back("QUERY_STRING=" + _quStr);
    envVect.push_back("CONTENT_LENGTH=1024");

    for (int i = 0; i < 7; i++)
        env[i] = (char *)envVect[i].c_str();
    env[7] = NULL;
}

void    Cgi::set4Post()
{
    char    **env = new char*[3];

    std::vector<std::string>    envVect;

    envVect.push_back("REQUEST_METHOD=" + _method);
    envVect.push_back("CONTENT_LENGTH=1024");

    for (int i = 0; i < 3; i++)
        env[i] = (char *)envVect[i].c_str();
    env[3] = NULL;

    if (dup2(_fd[1], STDIN_FILENO) == -1)
                throw std::runtime_error("dup error");
    if (dup2(_fd[0], STDIN_FILENO) == -1)
                throw std::runtime_error("dup error");

    std::cout << _quStr << std::endl;
}



/* std::string Cgi::initCgi()  
{
    pid_t       pid;
    int         status;
    int         fd[2];
    std::string	resp = "";

    if (pipe(fd) == -1)
        throw std::runtime_error("pipe failed");
    pid = fork();
    if (pid == -1)
        throw std::runtime_error("fork failed");
    else if (pid == 0) //child process
        selCgi(fd);
    //else //parent process
    waitpid(pid, &status, 0);
    close(fd[1]);

    char    buffer[1024];
    while (ssize_t bytes = read(fd[0], buffer, sizeof(buffer)))
    {
        if (bytes < 0)
            throw std::runtime_error("fd read error");
        buffer[bytes] = '\0';
        resp += buffer;
    }
    close(fd[0]);
    return (resp);
}


void Cgi::selCgi(int *fd)
{    
    close(fd[0]);
    if (dup2(fd[1], STDOUT_FILENO) == -1)
        throw std::runtime_error("dup failed");
    if (cgiExtension == "py")
        execPy();
    else if (cgiExtension == "sh")
        execSh();
    
}

void    Cgi::execPy()
{
    char    **cmdargs = setArgs();
    
    if (execve("/usr/local/bin/python3", cmdargs, NULL) == -1)
    {
        std::cerr << "cgi failed" << std::endl;
        delete cmdargs;
        exit;
    }
}

void    Cgi::execSh()
{
    char    **cmdargs = setArgs();
    
    if (execve("/bin/bash", cmdargs, NULL) == -1)
    {
        std::cerr << "cgi failed" << std::endl;
        delete cmdargs;
        exit;
    }
} */

std::string getPath(const std::string &line)
{
    size_t start = line.find("/");
    if (start == std::string::npos)
        throw std::runtime_error("invalid cgi request line"); //check error exit
    
    size_t end = line.find("?", start);
    if (end == std::string::npos)
        throw std::runtime_error("invalid cgi request line");

    std::string result = line.substr(start, end - start);

    return (result);
}

std::string getQuStr(const std::string &line)
{
    size_t start = line.find("/");
    if (start == std::string::npos)
        throw std::runtime_error("invalid cgi request line"); //check error exit
    
    size_t end = line.find("?", start);
    if (end == std::string::npos)
        throw std::runtime_error("invalid cgi request line");

    std::string result = line.substr(start, end - start);

    return (result);
}

