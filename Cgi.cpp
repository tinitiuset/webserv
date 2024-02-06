#include "Cgi.hpp"
#include <fcntl.h>
#include <unistd.h>


Cgi::Cgi(): cgiExtension("py"), cmd("python3"), script("cgi-bin/sos.py")
{
    arg  = "cagon tus muelas";

    //open socket file descriptor as the client socket will come from the server
    socket = open("cgi-bin/socket", O_WRONLY);
    if (socket == -1)
        throw std::runtime_error("open error");
    
}

Cgi::~Cgi()
{
    close(socket);
}

void Cgi::initCgi()
{
    pid_t   pid;
    int     status;

    pid = fork();
    if (pid == -1)
        throw std::runtime_error("fork failed");
    else if (pid == 0) //child process
        selCgi();
    else //parent process
        waitpid(pid, &status, 0);

}

char    **Cgi::setArgs()
{
    char    **cmdargs = new char*[3];

    cmdargs[0] = const_cast<char*>(cmd.c_str());
    cmdargs[1] = const_cast<char*>(script.c_str());
    cmdargs[2] = const_cast<char*>(arg.c_str());
    cmdargs[3] = NULL;

    return (cmdargs);
}

void Cgi::selCgi()
{    
    if (dup2(socket, STDOUT_FILENO) == -1)
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
        throw std::runtime_error("cgi failed");
        delete cmdargs;
    }
}

void    Cgi::execSh()
{
    char    **cmdargs = setArgs();
    
    if (execve("/bin/bash", cmdargs, NULL) == -1)
    {
        throw std::runtime_error("cgi failed");
        delete cmdargs;
    }
}