#include "Multiplexer.hpp"
#include "Server.hpp"
#include <unistd.h>
#include <sys/_select.h>
//#include <sys/select.h>

Multiplexer::Multiplexer()
{

	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);
}

Multiplexer::~Multiplexer() {
}

void genericResponse(const int fd) {
	char buffer[1024] = {0};
	read(fd, buffer, 1024);
	Logger::debug("Received from client: " + std::string(buffer));

	const std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello from server";
	write(fd, response.c_str(), response.size());

	Logger::debug("Response sent to client");

}

void Multiplexer::run(const Server &server) 
{
	int	selectRes;

	fd_set	readSet;
	fd_set	writeSet;
	int max_fd = getMaxFd(server.getSocketFd());

	while (!_endserver)
	{
        // Select for I/O multiplexing
		
		
		fd_set tmpReadSet = readSet;
		fd_set tmpWriteSet = writeSet;
		
		selectRes = select(max_fd + 1, &tmpReadSet, &tmpWriteSet, NULL, NULL);
		
		if (selectRes == 0)
			std::cout << "Selection OK" << std::endl;
		else if (selectRes == -1)
		{
			if (!_endserver)
				throw std::runtime_error("Select failed\n"); 
			continue ;
		}
		else
		{
			int fd = 3;
			
			while (fd <= max_fd)
			{
				if (FD_ISSET(fd, &tmpReadSet))
				{
					if (fd == server.getSocketFd()[0][0])
					{
						int new_fd = server.Accept(0, 0);
						FD_SET(new_fd, &readSet);
						if (new_fd > max_fd)
							max_fd = new_fd;
					}
					else
					{
						genericResponse(fd);
						close(fd);
						FD_CLR(fd, &readSet);
					}
				}
				else if (FD_ISSET(fd, &tmpWriteSet))
				{
					if (fd == server.getSocketFd()[0][0])
					{
						int new_fd = server.Accept(0, 0);
						FD_SET(new_fd, &writeSet);
						if (new_fd > max_fd)
							max_fd = new_fd;
					}
					else
					{
						genericResponse(fd);
						close(fd);
						FD_CLR(fd, &writeSet);
					}
				}
				fd++;
			}

		}
	}
}




		//-----------
      
        /* fd_set read_fds;
        FD_ZERO(&read_fds);

        for (size_t i = 0; i < server.getSocketFd().size(); ++i) {
            for (size_t j = 0; j < server.getSocketFd()[i].size(); ++j) {
                FD_SET(server.getSocketFd()[i][j], &read_fds);
            }
		}

        fd_set temp_fds = read_fds;

        if (select(max_fd + 1, &temp_fds, NULL, NULL, NULL) == -1) {
            std::cout << "Select failed\n" << std::endl;
			continue ;
			//throw std::runtime_error("Select failed\n");
        }
		//std::cout << "--- while if. max_fd: " << max_fd << std::endl;
        for (size_t i = 0; i < server.getSocketFd().size(); ++i) {
            for (size_t j = 0; j < server.getSocketFd()[i].size(); ++j) {
                int current_socket = server.getSocketFd()[i][j];
                if (FD_ISSET(current_socket, &temp_fds)) {
                    if (current_socket == server.getSocketFd()[i][j]) {
                        // Nuevo cliente
                        const int new_fd = server.Accept(i, j);
						std::cout << "*** if. socketFds[i][j]: " << current_socket << std::endl;
						std::cout << "*** if. new_fd: " << new_fd << std::endl;
						std::cout << "*** if. max_fd: " << max_fd << std::endl;
                        FD_SET(new_fd, &read_fds);
                        if (new_fd > max_fd) {
                            max_fd = new_fd;
                        }
                    } else {
                        // Cliente existente
						std::cout << "~~~ else: " << std::endl;
                        genericResponse(current_socket);
                        close(current_socket);
                        FD_CLR(current_socket, &read_fds);
                    }
                }
            }
        }
    }
} */
	/* for (int k = 0; k <= max_fd; k++) {
					if (FD_ISSET(k, &read_fds)) {
						if (k == socketFds[i][j])
						{
							int new_fd = server.Accept(i, j);
							std::cout << "*** if. socketFds[i][j]: " << socketFds[i][j] << std::endl;
							std::cout << "*** if. new_fd: " << new_fd << std::endl;
							std::cout << "*** if. max_fd: " << max_fd << std::endl;
							FD_SET(new_fd, &read_fds);
							if (new_fd > max_fd) {
								max_fd = new_fd;
							}
						}
						else
						{
							std::cout << "~~~ else: " << std::endl;
							// Currently writing generic responses
							genericResponse(k);
							close(k);
							FD_CLR(k, &read_fds); */

	/* while (true)
	{
		fd_set temp_fds = read_fds;

		// https://man7.org/linux/man-pages/man2/select.2.html
		if (select(max_fd + 1, &temp_fds, NULL, NULL, NULL) == -1)
			throw std::runtime_error("Select failed\n");

		for (int i = 0; i <= max_fd; i++) {
			if (FD_ISSET(i, &temp_fds)) {
				if (i == server.getSocketFd())
				{
					const int new_fd = server.Accept();
					FD_SET(new_fd, &read_fds);
					if (new_fd > max_fd) {
						max_fd = new_fd;
					}
				}
				else
				{
					// Currently writing generic responses
					genericResponse(i);
					close(i);
					FD_CLR(i, &read_fds);
				}
			}
		}
	} */

int	getMaxFd(std::vector<std::vector<int> > sockfd)
{
	int max_fd = 0;

	for (size_t i = 0; i < sockfd.size(); ++i)
	{
		if (!sockfd[i].empty())
		{
			int maxInVector = *std::max_element(sockfd[i].begin(), sockfd[i].end());
			if (maxInVector > max_fd)
				max_fd = maxInVector;
		}
	}
	return (max_fd);
}

bool	Multiplexer::_endserver = false;

void	Multiplexer::signalHandler(int signal)
{
	if (signal == SIGINT || signal == SIGTERM)
		_endserver = true;
}

size_t	Multiplexer::getServerFdIdx(int fd) const
{
	for (size_t i = 0; i < serverFdVec.size(); ++i)
		if (fd == serverFdVec[i])
			return (i);
	return (-1);
}

size_t	Multiplexer::getClientFdIdx(int fd) const
{
	for (size_t i = 0; i < clientFdVec.size(); ++i)
		if (fd == clientFdVec[i])
			return (i);
	return (-1);
}

void	Multiplexer::setServerFdVec(std::vector<std::vector<int> > sockfd)
{
	for (size_t i = 0; i < sockfd.size(); ++i)
		for (size_t j = 0; j < sockfd[i].size(); ++j)
			serverFdVec.push_back(sockfd[i][j]);
}

