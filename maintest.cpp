#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main()
{
	int		fd[2];
	pid_t	pid;
	/* const char *args[3];
	args[0] = "python3";
	args[1] = "test.py";
	args[2] = NULL; */
	char *const args[3] = {const_cast<char*>("python3"), const_cast<char*>("cgi-bin/test.py"), NULL};


	pipe(fd);

	std::cout << "tamos en el padre" << std::endl;
	
	write(fd[1], "caraculo\n", 9);
	pid = fork();
	close(fd[1]);
	if (pid == -1)
	{
		std::cerr << "fork error" << std::endl;
		return (1);
	}
	else if (pid == 0)
	{
		std::cout << "tamos en el hijo" << std::endl;

		//close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		//close(fd[0]);
		execve("/usr/bin/python3", args, NULL);
	}
	waitpid(pid, NULL, 0);
	
	std::string	resp = "";
	char	buffer[1024];
	while (ssize_t bytes = read(fd[0], buffer, sizeof(buffer)))
	{
		if (bytes < 0)
		{
			std::cerr << "read error" << std::endl;
			return (1);
		}
		buffer[bytes] = '\0';
		resp += buffer;
	}
	close(fd[0]);
	std::cout << "father again: " << resp << std::endl;
	





	return (0);
}