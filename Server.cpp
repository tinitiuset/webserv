#include "Server.hpp"

Server::Server(){
	
	int j = 0;

	for (int i = 0; i < confG->_servAmount; i++)
	{

		std::vector<int> ports = confG->_serverArr[i].getPorts();
		checkRepeatedPorts(ports); 
		std::vector<int>::iterator it;
		std::vector<int> row;
		sockfd.push_back(row);
		j = 0;
		for (it = ports.begin(); it != ports.end(); ++it)
		{
			sockfd[i].push_back(-1);
			socketOps(*it, i, j);
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

	for (size_t i = 0; i < sockfd.size(); ++i)
		for (size_t j = 0; j < sockfd[i].size(); ++j)
			close(sockfd[i][j]);
	
	/* std::vector<int>::iterator it;
	for (it = sockfd.begin(); it != sockfd.end(); ++it)
		close(*it); */
}

int Server::Accept(size_t i, size_t j) const {

	sockaddr_in cli = {};
	socklen_t len = sizeof(cli);

	// https://man7.org/linux/man-pages/man2/accept.2.html
	const int connfd = accept(sockfd[i][j], reinterpret_cast<struct sockaddr*>(&cli), &len);
	(connfd < 0)
		? throw std::runtime_error("Server accept failed\n")
		: Logger::debug("Server accepted the client");
	return connfd;
}

std::vector<std::vector<int> >	Server::getSocketFd() const {
	return sockfd;
}

void	Server::socketOps(int port, int i, int j)
{
	((sockfd[i][j] = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		? throw std::runtime_error("Socket creation failed\n")
		: Logger::debug("Socket successfully created");

	//Assign IP and port
	// AF_INET: IPv4
	// INADDR_ANY: any address for binding
	// htons: converts port number in host byte order to a port number in network byte order
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);

	// https://man7.org/linux/man-pages/man2/bind.2.html
	(bind(sockfd[i][j], reinterpret_cast<struct sockaddr*>(&servaddr), sizeof(servaddr)) == -1)
		? throw std::runtime_error("Binding failed\n")
		: Logger::debug("Socket successfully binded");

	// https://man7.org/linux/man-pages/man2/listen.2.html
	try {
		listen(sockfd[i][j], BACKLOG);
		Logger::info("Server listening on port " + std::to_string(port)); //OJO to_string cpp11?
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
}

void	checkRepeatedPorts(const std::vector<int> &vector)
{
    for (size_t i = 0; i < vector.size(); ++i)
        for (size_t j = i + 1; j < vector.size(); ++j)
            if (vector[i] == vector[j])
                throw std::runtime_error("Repeated ports in config file\n"); 
}

