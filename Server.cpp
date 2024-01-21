#include "Server.hpp"

Server::Server() : servaddr() {
	// https://man7.org/linux/man-pages/man2/socket.2.html
	// AF_INET: IPv4
	// SOCK_STREAM: TCP
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw std::runtime_error("Socket creation failed\n");
	else
		std::cout << "Socket successfully created\n";

	// Assign IP and port
	// AF_INET: IPv4
	// INADDR_ANY: any address for binding
	// htons: converts port number in host byte order to a port number in network byte order
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// https://man7.org/linux/man-pages/man2/bind.2.html
	(bind(sockfd, reinterpret_cast<struct sockaddr*>(&servaddr), sizeof(servaddr)) == -1)
		? throw std::runtime_error("Binding failed\n")
		: std::cout << "Socket successfully binded\n";

	// https://man7.org/linux/man-pages/man2/listen.2.html
	try {
		listen(sockfd, 5);
		std::cout << "Server listening on port " << PORT << "\n";
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
}

Server::~Server() {
	close(sockfd);
}
