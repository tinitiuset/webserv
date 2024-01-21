#include "Server.hpp"

Server::Server() : servaddr() {
	// https://man7.org/linux/man-pages/man2/socket.2.html
	// AF_INET: IPv4
	// SOCK_STREAM: TCP
	((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		? throw std::runtime_error("Socket creation failed\n")
		: Logger::debug("Socket successfully created");

	// Assign IP and port
	// AF_INET: IPv4
	// INADDR_ANY: any address for binding
	// htons: converts port number in host byte order to a port number in network byte order
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERVER_PORT);

	// https://man7.org/linux/man-pages/man2/bind.2.html
	(bind(sockfd, reinterpret_cast<struct sockaddr*>(&servaddr), sizeof(servaddr)) == -1)
		? throw std::runtime_error("Binding failed\n")
		: Logger::debug("Socket successfully binded");

	// https://man7.org/linux/man-pages/man2/listen.2.html
	try {
		listen(sockfd, 5);
		Logger::info("Server listening on port " + std::to_string(SERVER_PORT));
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
}

Server::~Server() {
	Logger::info("Server shutting down");
	close(sockfd);
}

int Server::Accept() const {

	sockaddr_in cli = {};
	socklen_t len = sizeof(cli);

	// https://man7.org/linux/man-pages/man2/accept.2.html
	const int connfd = accept(sockfd, reinterpret_cast<struct sockaddr*>(&cli), &len);
	(connfd < 0)
		? throw std::runtime_error("Server accept failed\n")
		: Logger::debug("Server accepted the client");
	return connfd;
}

int Server::getSocketFd() const {
	return sockfd;
}

