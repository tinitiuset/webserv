#include <fcntl.h>
#include <unistd.h>
#include <iostream>

int main(int ac, char **av, char **env)
{
    char    **cmdargs = new char*[3];

    cmdargs[0] = "sos.py";
    cmdargs[1] = NULL;

    if (execve("sos.py", cmdargs, env) == -1)
        std::cout << "holi" << std::endl;

    return (0);

}