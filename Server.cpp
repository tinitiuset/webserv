#include "Server.hpp"

Server::Server(){
	
	int j = 0;

	for (int i = 0; i < confG->_servAmount; i++)
	{
		std::vector<int> ports = confG->_serverArr[i].getPorts();
		std::vector<int>::iterator it;
		for (it = ports.begin(); it != ports.end(); ++it)
		{
			sockfd.push_back(-1);
			std::cout << *it << std::endl;
			socketOps(*it, j);
			j++;
		}
	}

	

	/* ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		? throw std::runtime_error("Socket creation failed\n")
		: Logger::debug("Socket successfully created");

	// Assign IP and port
	// AF_INET: IPv4
	// INADDR_ANY: any address for binding
	// htons: converts port number in host byte order to a port number in network byte order
	struct sockaddr_in servaddr;
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
		//Logger::info("Server listening on port " + std::to_string(SERVER_PORT)); //OJO to_string cpp11?
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	} */
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

std::vector<int>	Server::getSocketFd() const {
	return sockfd;
}

void	Server::socketOps(int port, int i)
{
	((sockfd[i] = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		? throw std::runtime_error("Socket creation failed\n")
		: Logger::debug("Socket successfully created");

	// Assign IP and port
	// AF_INET: IPv4
	// INADDR_ANY: any address for binding
	// htons: converts port number in host byte order to a port number in network byte order
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);

	// https://man7.org/linux/man-pages/man2/bind.2.html
	(bind(sockfd[i], reinterpret_cast<struct sockaddr*>(&servaddr), sizeof(servaddr)) == -1)
		? throw std::runtime_error("Binding failed\n")
		: Logger::debug("Socket successfully binded");

	// https://man7.org/linux/man-pages/man2/listen.2.html
	try {
		listen(sockfd[i], 5);
		//Logger::info("Server listening on port " + std::to_string(SERVER_PORT)); //OJO to_string cpp11?
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
}