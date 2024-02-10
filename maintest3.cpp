#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctime>
#include <csignal>

int main()
{
	int i = 0;

    int fd_parent_to_child[2];
    int fd_child_to_parent[2];
    pid_t pid;
    std::string resp = "RESP ";
    char buffer[1024];

    char *const args[3] = {const_cast<char*>("python3"), const_cast<char*>("cgi-bin/test.py"), NULL};

    // Crear las dos tuberías
	if (i == 1)
	{
		if (pipe(fd_parent_to_child) == -1)  // if POST ***
		{
			std::cerr << "pipe error" << std::endl;
			return 1;
		}
	}
	if (pipe(fd_child_to_parent) == -1) 
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
        if (i == 1)
			close(fd_parent_to_child[1]); // if POST ***
		
        close(fd_child_to_parent[0]);

        // Redirige la entrada estándar al extremo de escritura de la tubería padre-hijo
        if (i == 1)
			dup2(fd_parent_to_child[0], STDIN_FILENO); // if POST ***

        // Redirige la salida estándar al extremo de lectura de la tubería hijo-padre
        dup2(fd_child_to_parent[1], STDOUT_FILENO);

        // Cierra los extremos del pipe
        if (i == 1)
			close(fd_parent_to_child[0]); // if POST ***
        close(fd_child_to_parent[1]);

        // Ejecuta el script de Python
        execve("/usr/bin/python3", args, NULL);
    }
    else  // Proceso padre
    {
        // Cierra los extremos no utilizados
        if (i == 1)
			close(fd_parent_to_child[0]); // if POST ***
        close(fd_child_to_parent[1]);

        // Escribe en la tubería padre-hijo
		if (i == 1)
		{
			write(fd_parent_to_child[1], "caraculo", 8); // if POST ***
			close(fd_parent_to_child[1]);				// if POST ***
		}

        // Obtiene el tiempo de inicio
        std::time_t start_time = std::time(NULL);

        // Espera a que el hijo termine (no bloqueante)
        int status;
        while (waitpid(pid, &status, WNOHANG) == 0)
        {
            // Obtiene el tiempo actual
            std::time_t current_time = std::time(NULL);

            // Comprueba si ha pasado más de 5 segundos
            if (current_time - start_time > 5)  // Ajusta el límite de tiempo según tus necesidades
            {
                std::cout << "Timeout alcanzado. Matando al proceso hijo." << std::endl;
                kill(pid, SIGKILL);
                break;
            }

            // Espera un breve periodo antes de volver a verificar
            usleep(100000);  // 100,000 microsegundos (0.1 segundos)
        }

        // Lee desde la tubería hijo-padre
        ssize_t bytes;

        while ((bytes = read(fd_child_to_parent[0], buffer, sizeof(buffer))) > 0)
        {
            buffer[bytes] = '\0';
            resp += buffer;
        }

        close(fd_child_to_parent[0]);

        std::cout << "Padre otra vez:\n" << resp << std::endl;
    }

    return 0;
}
