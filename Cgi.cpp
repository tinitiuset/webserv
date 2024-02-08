#include "Cgi.hpp"
#include <fcntl.h>
#include <unistd.h>


Cgi::Cgi(): cgiExtension("py"), cmd("python3"), script("cgi-bin/sos.py"), arg("input_text=hola+cara+culo+ajeroso")
{
    //open socket file descriptor as the client socket will come from the server
    socket = open("cgi-bin/socket", O_WRONLY);
    if (socket == -1)
        throw std::runtime_error("open error");
    
}

Cgi::~Cgi(){}

std::string Cgi::initCgi()  //CHECK ERROR MANAGEMENT
{
    pid_t       pid;
    int         status;
    int         fd[2];
    std::string	resp = "";

    if (pipe(fd) == -1)
    {
        throw std::runtime_error("pipe failed");

    }
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

char    **Cgi::setArgs()
{
    char    **cmdargs = new char*[3];

    cmdargs[0] = const_cast<char*>(cmd.c_str());
    cmdargs[1] = const_cast<char*>(script.c_str());
    cmdargs[2] = const_cast<char*>(arg.c_str());
    cmdargs[3] = NULL;

    return (cmdargs);
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
        throw std::runtime_error("cgi failed");
        delete cmdargs;
    }
}