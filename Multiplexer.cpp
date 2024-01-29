#include "Multiplexer.hpp"

#include <unistd.h>
#include <sys/_select.h>

#include "Request.hpp"
#include "GetRequest.hpp"
#include "PostRequest.hpp"

Multiplexer::Multiplexer() {}

Multiplexer::~Multiplexer() {}

Request* createRequest(const int &fd) {
	Request temp;
	temp.parseRequest(fd);

	if(temp.isGetRequest())
		return new GetRequest(temp);
	else if (temp.isPostRequest())
		return new PostRequest(temp);

	return NULL;
}

void Multiplexer::run(const Server &server) {

	int max_fd = server.getSocketFd();

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
					// Remember to delete the request after use
					Request* request = createRequest(i);
					if (request == NULL)
						throw std::runtime_error("Request is not POST nor GET");
					std::string response = request->handle();
					Logger::debug("Multiplexer::run() sending response of size " + std::to_string(response.length()));
					write(i, response.c_str(), response.length());
					delete request;
					close(i);
					FD_CLR(i, &read_fds);
				}
			}
		}
	}
}