#include "Multiplexer.hpp"
#include "Server.hpp"
#include "Logger.hpp"
#include <unistd.h>
//#include <sys/_select.h>
#include <sys/select.h>
#include <csignal>
#include <algorithm>

#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>


//#include <sys/select.h>

void genericResponse(const int fd) {
	char buffer[1024] = {0};
	read(fd, buffer, 1024);
	Logger::debug("Received from client: " + std::string(buffer));

	const std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello from server";
	write(fd, response.c_str(), response.size());

	Logger::debug("Response sent to client");

}

Multiplexer::Multiplexer()
{
	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);
}

Multiplexer::~Multiplexer() {}

void Multiplexer::run(const Server &server) 
{
	int	selectRes;

	fd_set	readSet;
	fd_set	writeSet;

	FD_ZERO(&readSet);
	FD_ZERO(&writeSet);

	std::cout << std::endl << "--------------" << std::endl << std::endl;

	setServerFdVec(server.getSocketFd());

	std::cout << "Server fd vector size: " << serverFdVec.size() << std::endl;
	std::cout << "Server fd vec[]: ";
	for (size_t i = 0; i < serverFdVec.size(); ++i)
	{
		FD_SET(serverFdVec[i], &readSet);
		std::cout << serverFdVec[i] << " ";
	}

	std::cout << std::endl;

	std::cout << "Client fd vector: ";
	for (size_t i = 0; i < clientFdVec.size(); ++i)
		std::cout << clientFdVec[i] << " ";
	
	std::cout << std::endl;

	int max_fd = getMaxFd(server.getSocketFd());
	std::cout << "Max fd: " << max_fd << std::endl;

	std::cout << std::endl << "--------------" << std::endl << std::endl;


	while (!_endserver)
	{
        std::cout << "\nNEW WHILE ITERATION\n" << std::endl;
		//Select for I/O multiplexing
		sleep(2);
		fd_set tmpReadSet = readSet;
		fd_set tmpWriteSet = writeSet;
		
		selectRes = select(max_fd + 1, &tmpReadSet, &tmpWriteSet, NULL, NULL);
		std::cout << "Select result: " << selectRes << std::endl;

		if (selectRes == 0)
			std::cout << "Selection OK" << std::endl;
		else if (selectRes == -1)
		{
			if (!_endserver)
				throw std::runtime_error("Select failed\n"); 
			continue ;
		}
		else if (selectRes > 0)
		{
			int fd = 3;
			
			while (fd <= max_fd)
			{
				std::cout << "\nNEW FD <= max_fd ITERATION\n" << std::endl;
				int	locReadVec = getServerFdIdx(fd);
				int	locWriteVec = getClientFdIdx(fd);

				std::cout << "locReadVec: " << locReadVec << " - locWriteVec: " << locWriteVec << std::endl;
				
				std::cout << "socket: " << fd << " - max_fd: " << max_fd << std::endl;

				if (FD_ISSET(fd, &tmpReadSet))
				{
					std::cout << ">>cumple FD_ISSET(fd, &tmpReadSet)" << std::endl;
					if (locReadVec != -1)
					{
						std::cout << "\n*  IN ACCEPT" << std::endl;
						// ACCEPT REQUEST 
						struct sockaddr 	cli;
						socklen_t	clilen = sizeof(cli);
						int			cliFd;

						cliFd = accept(fd, (struct sockaddr *)&cli, &clilen);
						cliFd == -1
						? throw std::runtime_error("Failed to accept\n")
						: Logger::debug("Client socket accepted by server\n");

						std::cout << "Client fd: " << cliFd << std::endl;
						
						//print server vector
						std::cout << "Server fd vector: ";
						for (size_t i = 0; i < serverFdVec.size(); ++i)
							std::cout << serverFdVec[i] << " ";
						std::cout << std::endl;
						//print client vector
						std::cout << "Client fd vector: ";
						for (size_t i = 0; i < clientFdVec.size(); ++i)
							std::cout << clientFdVec[i] << " ";
						std::cout << std::endl;


						(fcntl(cliFd, F_SETFL, O_NONBLOCK) == -1)
						? throw std::runtime_error("Failed to set client socket as non-blocked\n0")
						: Logger::debug("Client socket set as non-blocking\n");

						clientFdVec.push_back(cliFd);
						std::cout << "pushed back clientFdVec: " << cliFd << std::endl;
						/*
						if any config data is needed here we can call
						size_t *getSockFdCoords(int fd) and store result in int i and int j
						i for server index 
						j for nested vectors index
						*/
						std::cout << "FD_SET cliFd: " << cliFd << std::endl;
						FD_SET(cliFd, &readSet);
						if (cliFd > max_fd)
							max_fd = cliFd;

						

						//Logger::debug("Client connected\n");
						std::cout << "clifd: " << cliFd << " - max_fd: " << max_fd << std::endl;
						std::cout << "Server fd vector: ";
						for (size_t i = 0; i < serverFdVec.size(); ++i)
							std::cout << serverFdVec[i] << " ";
						std::cout << std::endl;
						//print client vector
						std::cout << "Client fd vector: ";
						for (size_t i = 0; i < clientFdVec.size(); ++i)
							std::cout << clientFdVec[i] << " ";
						std::cout << std::endl;
					}
					else if(locWriteVec != -1)
					{
						//READ REQUEST...
						std::cout << "\n*  IN READ REQUEST" << std::endl;
						std::cout << "FD_CLR(clientFdVec[locWriteVec], &tmpReadSet) " << clientFdVec[locWriteVec] << std::endl;
						FD_CLR(clientFdVec[locWriteVec], &readSet);
						//if se ha leido una solicitud y hay datos para enviar:
							//FD_SET(clientFdVec[locWriteVec], &tmpWriteSet);
						FD_SET(clientFdVec[locWriteVec], &writeSet);
						std::cout << "FD_SET clientFdVec[" << locWriteVec << "]: " << clientFdVec[locWriteVec] << std::endl;
						std::cout << "Server fd vector: ";
						for (size_t i = 0; i < serverFdVec.size(); ++i)
							std::cout << serverFdVec[i] << " ";
						std::cout << std::endl;
						//print client vector
						std::cout << "Client fd vector: ";
						for (size_t i = 0; i < clientFdVec.size(); ++i)
							std::cout << clientFdVec[i] << " ";
						std::cout << std::endl;
					}
				}
				else if (FD_ISSET(fd, &tmpWriteSet))
				{
					std::cout << ">>cumple FD_ISSET(fd, &tmpWriteSet)" << std::endl;
					//SEND RESPONSE
					//Logger::debug("Fd ready to send data");
					std::cout << "\n*  IN SEND RESPONSE clientFdVec[" << locWriteVec << "]: " << clientFdVec[locWriteVec] << std::endl;
					std::cout << "fd: " << fd << " max_fd: " << max_fd << std::endl;
					std::cout << "Server fd vector: ";
					for (size_t i = 0; i < serverFdVec.size(); ++i)
						std::cout << serverFdVec[i] << " ";
					std::cout << std::endl;
					//print client vector
					std::cout << "Client fd vector: ";
					for (size_t i = 0; i < clientFdVec.size(); ++i)
						std::cout << clientFdVec[i] << " ";
					std::cout << std::endl;
					std::cout << "generic responsa called" << std::endl;
					genericResponse(fd);
					std::cout << "close clientFdVec[" << locWriteVec << "]: " << clientFdVec[locWriteVec] << std::endl;
					close(clientFdVec[locWriteVec]);
					if (clientFdVec[locWriteVec] == max_fd)
						max_fd--;
					std::cout << "max_fd: " << max_fd << std::endl;
					std::cout << "FD_CLR clientFdVec[" << locWriteVec << "]: " << clientFdVec[locWriteVec] << std::endl;
					FD_CLR(clientFdVec[locWriteVec], &writeSet);
					clientFdVec.erase(clientFdVec.begin() + locWriteVec);
					std::cout << "erased at client fd vector: ";
					for (size_t i = 0; i < clientFdVec.size(); ++i)
					{
						std::cout << clientFdVec[i] << " ";
					}
					std::cout << std::endl;
				}
				fd++;
			}
		}
	}
}




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

