#include "Multiplexer.hpp"
#include "utils/Logger.hpp"
#include <unistd.h>
#include <sys/select.h>
#include <csignal>
#include <algorithm>

#include "requests/Request.hpp"
#include "requests/GetRequest.hpp"
#include "requests/PostRequest.hpp"
#include "requests/DeleteRequest.hpp"

//Request* createRequest(const int &fd, const std::list <Location*> locations) {
Request* createRequest(const int &fd){
	//Request temp (fd, locations);
	Request temp;
	temp.parseRequest(fd);

	if(temp.isGetRequest())
		return new GetRequest(temp);
	else if (temp.isPostRequest())
		return new PostRequest(temp);
	else if (temp.isDeleteRequest())
		return new DeleteRequest(temp);

	return NULL;
}

Multiplexer::Multiplexer()
{
	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);
}

Multiplexer::~Multiplexer() {}

void Multiplexer::run()
{
	int max_fd = conf->serverCount() + 2;

	fd_set	readSet;
	fd_set	writeSet;

	FD_ZERO(&readSet);
	FD_ZERO(&writeSet);

	serverFdVec = conf->serverSockets();

	for (size_t i = 0; i < serverFdVec.size(); ++i)
		FD_SET(serverFdVec[i], &readSet);

	while (!_endserver)
	{
		fd_set tmpReadSet = readSet;
		fd_set tmpWriteSet = writeSet;

		const int selectRes = select(max_fd + 1, &tmpReadSet, &tmpWriteSet, NULL, NULL);

		if (selectRes == 0)
			std::cout << "Selection OK" << std::endl;
		else if (selectRes == -1)
		{
			if (!_endserver)
				throw std::runtime_error("Select failed\n");
		}
		else if (selectRes > 0)
		{
			int fd = 3;

			while (fd <= max_fd)
			{
				const size_t locReadVec = getServerFdIdx(fd);
				const size_t locWriteVec = getClientFdIdx(fd);

				if (FD_ISSET(fd, &tmpReadSet))
				{
					if (locReadVec != -1)
					{
						sockaddr 	cli = {};
						socklen_t	clilen = sizeof(cli);
						int			cliFd;

						cliFd = accept(fd, &cli, &clilen);
						cliFd == -1
						? throw std::runtime_error("Failed to accept\n")
						: Logger::debug("Client socket accepted by server\n");

						(fcntl(cliFd, F_SETFL, O_NONBLOCK) == -1)
						? throw std::runtime_error("Failed to set client socket as non-blocked\n0")
						: Logger::debug("Client socket set as non-blocking\n");

						clientFdVec.push_back(cliFd);

						FD_SET(cliFd, &readSet);
						if (cliFd > max_fd)
							max_fd = cliFd;

						Logger::debug("Client connected\n");
					}
					else if(locWriteVec != -1)
					{
						FD_CLR(clientFdVec[locWriteVec], &readSet);
						FD_SET(clientFdVec[locWriteVec], &writeSet);
					}
				}
				else if (FD_ISSET(fd, &tmpWriteSet))
				{
					//Request* request = createRequest(fd, conf->getServer(0)._locations);
					Request* request = createRequest(fd);
					if (request == NULL)
						throw std::runtime_error("Request is not POST nor GET");
					std::string response = request->handle();
					Logger::debug("Multiplexer::run() sending response of size " + Utils::toString(response.length()));
					write(fd, response.c_str(), response.length());
					delete request;

					close(clientFdVec[locWriteVec]);
					if (clientFdVec[locWriteVec] == max_fd)
						max_fd--;
					FD_CLR(clientFdVec[locWriteVec], &writeSet);
					clientFdVec.erase(clientFdVec.begin() + locWriteVec);
				}
				fd++;
			}
		}
	}
	for (int i = 0; i < max_fd; ++i)
		close(i);
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

size_t	Multiplexer::getServerFdIdx(const int fd) const
{
	for (size_t i = 0; i < serverFdVec.size(); ++i)
		if (fd == serverFdVec[i])
			return (i);
	return (-1);
}

size_t	Multiplexer::getClientFdIdx(const int fd) const
{
	for (size_t i = 0; i < clientFdVec.size(); ++i)
		if (fd == clientFdVec[i])
			return (i);
	return (-1);
}

void	Multiplexer::setServerFdVec(const std::vector<int>& sockfd)
{
	for (size_t i = 0; i < sockfd.size(); ++i)
		serverFdVec.push_back(sockfd[i]);
}
