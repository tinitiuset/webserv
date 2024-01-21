#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <unistd.h>

#define PORT 8080

class Server {

private:
	int sockfd;
	struct sockaddr_in servaddr;

public:
	Server();
	~Server();
};

#endif
