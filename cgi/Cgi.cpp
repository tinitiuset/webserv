#include "Cgi.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include "../utils/defaults.hpp"

//CHECK ERROR MANAGEMENT


Cgi::Cgi(int servI, int port, std::string reqLine, std::map<std::string, std::string> headers, std::string body):
_servIdx(servI),
_port(port),
_reqLine(reqLine),
_headers(headers),
_body(body) 
{
    _env = NULL;
}


Cgi::~Cgi(){}

void    Cgi::selMethod()
{
    _fdOut = dup(STDOUT_FILENO);
    pipe(_fd);
    
    if (_reqLine.substr(0, 3) == "GET") //also check if method is supported in config????
    {
        _method = "GET";
        setGETEnv();
    }
    else if (_reqLine.substr(0, 4) == "POST") //also check if method is supported in config????
    {
        _method = "POST";
        setPost();
    }
    else
    {
        std::cerr << "method not supported" << std::endl;
        return ; //?
    }
}

void    Cgi::setGETEnv()
{
    char    **cmdargs = new char*[15];

    std::vector<std::string>    envVect;

    envVect[0] = "SERVER_NAME=" + confG->_serverArr[_servIdx].getServerName();
    envVect[1] = "GATEWAY_INTERFACE=CGI/1.1";
    envVect[2] = "SERVER_PROTOCOL=HTTP/1.1";
    envVect[3] = "SERVER_PORT="

    return (cmdargs);
}

std::string Cgi::initCgi()  
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
}