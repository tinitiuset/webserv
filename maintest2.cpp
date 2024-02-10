#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int fd_parent_to_child[2];
    int fd_child_to_parent[2];
    pid_t pid;

    char *const args[3] = {const_cast<char*>("python3"), const_cast<char*>("cgi-bin/test.py"), NULL};

    // Crear las dos tuberías
    if (pipe(fd_parent_to_child) == -1 || pipe(fd_child_to_parent) == -1)
    {
        std::cerr << "pipe error" << std::endl;
        return 1;
    }

    std::cout << "tamos en el padre" << std::endl;

    pid = fork();
    if (pid == -1)
    {
        std::cerr << "fork error" << std::endl;
        return 1;
    }
    else if (pid == 0)  // Proceso hijo
    {
        std::cout << "tamos en el hijo" << std::endl;

        // Cierra los extremos no utilizados
        close(fd_parent_to_child[1]);
        close(fd_child_to_parent[0]);

        // Redirige la entrada estándar al extremo de escritura de la tubería padre-hijo
        dup2(fd_parent_to_child[0], STDIN_FILENO);

        // Redirige la salida estándar al extremo de lectura de la tubería hijo-padre
        dup2(fd_child_to_parent[1], STDOUT_FILENO);

        // Ejecuta el script de Python
        execve("/usr/bin/python3", args, NULL);
    }
    else  // Proceso padre
    {
        // Cierra los extremos no utilizados
        close(fd_parent_to_child[0]);
        close(fd_child_to_parent[1]);

        // Escribe en la tubería padre-hijo
        write(fd_parent_to_child[1], "caraculo", 8);
        close(fd_parent_to_child[1]);

        // Espera a que el hijo termine
        waitpid(pid, NULL, 0);

        // Lee desde la tubería hijo-padre
        std::string resp = "";
        char buffer[1024];
        ssize_t bytes;

        while ((bytes = read(fd_child_to_parent[0], buffer, sizeof(buffer))) > 0)
        {
            buffer[bytes] = '\0';
            resp += buffer;
        }

        close(fd_child_to_parent[0]);

        std::cout << "Padre otra vez: " << resp << std::endl;
    }

    return 0;
}
