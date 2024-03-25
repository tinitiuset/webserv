#include "Multiplexer.hpp"

Request* Multiplexer::createRequest(const int& fd) {
	Request* temp = requestList.getRequest(fd);
	Request* retRequest = NULL;

	if (temp->isGetRequest())
	{
		retRequest = new GetRequest(*temp);
		requestList.removeRequest(fd);
	}
	else if (temp->isPostRequest())
	{
		retRequest = new PostRequest(*temp);
		requestList.removeRequest(fd);
	}
	else if (temp->isDeleteRequest())
	{
		retRequest = new DeleteRequest(*temp);
		requestList.removeRequest(fd);
	}

	return retRequest;
}

Multiplexer::Multiplexer() {
	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);
	_timeout.tv_sec = 10;
	_timeout.tv_usec = 10000;
}

Multiplexer::~Multiplexer() {
}

void Multiplexer::run() {
	int max_fd = conf->serverCount() + 2;

	fd_set readSet;
	fd_set writeSet;

	FD_ZERO(&readSet);
	FD_ZERO(&writeSet);

	serverFdVec = conf->serverSockets();

	for (size_t i = 0; i < serverFdVec.size(); ++i)
		FD_SET(serverFdVec[i], &readSet);

	while (!_endserver) {
		/* fd_set tmpReadSet = readSet;
		fd_set tmpWriteSet = writeSet; */
		sleep(10);
		const int selectRes = select(max_fd + 1, &readSet, &writeSet, NULL, &_timeout);

		if (selectRes == 0)
			std::cout << "Selection OK" << std::endl;
		else if (selectRes == -1) {
			if (!_endserver)
				throw std::runtime_error("Select failed\n");
		}
		else if (selectRes > 0) {
			int fd = 3;
			std::cout << "Selection RES: " << selectRes << std::endl;
			while (fd <= max_fd) {
				const int locReadVec = getServerFdIdx(fd);
				const int locWriteVec = getClientFdIdx(fd);

				if (FD_ISSET(fd, &readSet)) {
					if (locReadVec != -1 && (locWriteVec == -1)) {
						sockaddr cli = {};
						socklen_t clilen = sizeof(cli);
						int cliFd;

						cliFd = accept(serverFdVec[locReadVec], &cli, &clilen);
						cliFd == -1
							? throw std::runtime_error("Failed to accept\n")
							: Logger::debug("Client socket accepted by server\n");

						(fcntl(cliFd, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1)
							? throw std::runtime_error("Failed to set client socket as non-blocked\n0")
							: Logger::debug("Client socket set as non-blocking\n");

						clientFdVec.push_back(cliFd);

						FD_SET(cliFd, &readSet);
						if (cliFd > max_fd)
							max_fd = cliFd;

						Logger::debug("Client connected\n");
						Request* request = new Request(cliFd);
						requestList.addRequest(cliFd, request);
						request->read();
					}
					/* else if ((requestList.getRequest(fd)) && (requestList.getRequest(fd))->isReadComplete() == false) {
						(requestList.getRequest(fd))->read();
					} */
					else if ((locWriteVec != -1) && (requestList.getRequest(clientFdVec[locWriteVec]))->isReadComplete()) {
						(requestList.getRequest(clientFdVec[locWriteVec]))->parseRequest();
						FD_CLR(clientFdVec[locWriteVec], &readSet);
						FD_SET(clientFdVec[locWriteVec], &writeSet);
					}
				}
				else if (FD_ISSET(fd, &writeSet)) {
					Request* specRequest = createRequest(fd);
					if (specRequest != NULL) {
						std::string response = specRequest->handle();
						Logger::debug(
							"Multiplexer::run() sending response of size " + Utils::toString(response.length()));
						ssize_t bytesSent = 0;
						while (bytesSent < static_cast<long>(response.length())) {
							ssize_t result = send(fd, response.c_str() + bytesSent, response.length() - bytesSent, 0);
							if (result > 0)
								bytesSent += result;
						}
						delete specRequest;
					}
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
		_endserver = true;
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
