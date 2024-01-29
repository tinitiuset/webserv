#include "Multiplexer.hpp"
#include "Server.hpp"
#include <unistd.h>
#include <sys/_select.h>
//#include <sys/select.h>

Multiplexer::Multiplexer() {
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

void Multiplexer::run(const Server &server) {

	//std::vector<std::vector<int> > sockfd = server.getSocketFd();
	int	max_fd = getMaxFd(server.getSocketFd());

	fd_set read_fds;
	FD_ZERO(&read_fds);
	FD_SET(max_fd, &read_fds);

	while (true) {
		fd_set temp_fds = read_fds;

		// https://man7.org/linux/man-pages/man2/select.2.html
		if (select(max_fd + 1, &temp_fds, NULL, NULL, NULL) == -1)
			throw std::runtime_error("Select failed\n");

		for (int i = 0; i <= max_fd; i++) {
			if (FD_ISSET(i, &temp_fds)) {
				if (i == server.getSocketFd()) {
					const int new_fd = server.Accept();
					FD_SET(new_fd, &read_fds);
					if (new_fd > max_fd) {
						max_fd = new_fd;
					}
				} else {
					// Currently writing generic responses
					genericResponse(i);
					close(i);
					FD_CLR(i, &read_fds);
				}
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
}
