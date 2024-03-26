#include "Multiplexer.hpp"

Request* morphRequest(Request *request) {

	if (request->isGetRequest())
		return (new GetRequest(*request));
	else if (request->isPostRequest())
		return new PostRequest(*request);
	else if (request->isDeleteRequest())
		return new DeleteRequest(*request);
	return NULL;
}

Multiplexer::Multiplexer() {
	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);
}

Multiplexer::~Multiplexer() {
}

void Multiplexer::run() {
	int max_fd = conf->serverCount() + 2;

	fd_set readSet;
	fd_set writeSet;

	RequestList requestList;

	FD_ZERO(&readSet);
	FD_ZERO(&writeSet);

	serverFdVec = conf->serverSockets();

	for (size_t i = 0; i < serverFdVec.size(); ++i)
		FD_SET(serverFdVec[i], &readSet);

	while (!_endserver) {
		fd_set tmpReadSet = readSet;
		fd_set tmpWriteSet = writeSet;

		const int selectRes = select(max_fd + 1, &tmpReadSet, &tmpWriteSet, NULL, NULL);

		if (selectRes == 0)
			std::cout << "Selection OK" << std::endl;
		else if (selectRes == -1) {
			if (!_endserver)
				throw std::runtime_error("Select failed\n");
		}
		else if (selectRes > 0) {
			int fd = 3;

			while (fd <= max_fd) {
				const int locReadVec = getServerFdIdx(fd);
				const int locWriteVec = getClientFdIdx(fd);

				if (FD_ISSET(fd, &tmpReadSet)) {
					if (locReadVec != -1) {
						sockaddr cli = {};
						socklen_t clilen = sizeof(cli);
						int cliFd;

						cliFd = accept(fd, &cli, &clilen);
						cliFd == -1
							? throw std::runtime_error("Failed to accept\n")
							: Logger::debug("Client socket accepted by server\n");

						(fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1)
							? throw std::runtime_error("Failed to set client socket as non-blocked\n0")
							: Logger::debug("Client socket set as non-blocking\n");

						clientFdVec.push_back(cliFd);

						FD_SET(cliFd, &readSet);
						if (cliFd > max_fd)
							max_fd = cliFd;

						Logger::debug("Client connected\n");

						requestList.addRequest(new Request(cliFd));
					}
					else if (locReadVec == -1) {
						Request *req = requestList.getRequest(fd);
						if (req->read(9999) < 9999) {

							std::map<std::string, std::string>::iterator it = req->getHeaders().find("Expect");
							if (it != req->getHeaders().end() && it->second == "100-continue") {
								req->getHeaders().erase("Expect");
								req->parseBody();
							}
							else {
								req->parseRequest();

								if (!req->isGetRequest() && !req->isPostRequest() && !req->isDeleteRequest())
								{
									FD_CLR(fd, &readSet);
									FD_SET(fd, &writeSet);
									fd++;
									continue;
								}

								requestList.addRequest(morphRequest(req));
								requestList.removeRequest(fd);
							}

							req = requestList.getRequest(fd);
							if (req->getHeaders().find("Expect") == req->getHeaders().end())
								req->handle();

							req->parseRequest();

							if (!req->isGetRequest() && !req->isPostRequest() && !req->isDeleteRequest())
							{
								FD_CLR(fd, &readSet);
								FD_SET(fd, &writeSet);
								fd++;
								continue;
							}

							requestList.addRequest(morphRequest(req));
							requestList.removeRequest(fd);

							requestList.getRequest(fd)->handle();
							FD_CLR(fd, &readSet);
							FD_SET(fd, &writeSet);
						}
					}
				}
				else if (FD_ISSET(fd, &tmpWriteSet)) {
					std::map<std::string, std::string>::iterator it = requestList.getRequest(fd)->getHeaders().find("Expect");
					if (it != requestList.getRequest(fd)->getHeaders().end() && it->second == "100-continue" && requestList.getRequest(fd)->checkContentLength() == 0) {
						Response response;
						response.set_start_line("HTTP/1.1 100 Continue");
						send(fd, response.format().c_str(), response.format().length(), 0);
						FD_CLR(fd, &writeSet);
						FD_SET(fd, &readSet);
					} else if (requestList.getRequest(fd)->write() <= 0) {
						close(clientFdVec[locWriteVec]);
						requestList.removeRequest(fd);
						if (clientFdVec[locWriteVec] == max_fd)
							max_fd--;
						FD_CLR(clientFdVec[locWriteVec], &writeSet);
						clientFdVec.erase(clientFdVec.begin() + locWriteVec);
					}
				}
				fd++;
			}
		}
	}
	for (int i = 3; i < max_fd; ++i)
		close(i);
}

int getMaxFd(std::vector<std::vector<int> > sockfd) {
	int max_fd = 0;

	for (size_t i = 0; i < sockfd.size(); ++i) {
		if (!sockfd[i].empty()) {
			int maxInVector = *std::max_element(sockfd[i].begin(), sockfd[i].end());
			if (maxInVector > max_fd)
				max_fd = maxInVector;
		}
	}
	return (max_fd);
}

bool Multiplexer::_endserver = false;

void Multiplexer::signalHandler(int signal) {
	if (signal == SIGINT || signal == SIGTERM)
	{
		_endserver = true;
	}
}

size_t Multiplexer::getServerFdIdx(const int fd) const {
	for (size_t i = 0; i < serverFdVec.size(); ++i)
		if (fd == serverFdVec[i])
			return (i);
	return (-1);
}

size_t Multiplexer::getClientFdIdx(const int fd) const {
	for (size_t i = 0; i < clientFdVec.size(); ++i)
		if (fd == clientFdVec[i])
			return (i);
	return (-1);
}

void Multiplexer::setServerFdVec(const std::vector<int>&sockfd) {
	for (size_t i = 0; i < sockfd.size(); ++i)
		serverFdVec.push_back(sockfd[i]);
}
