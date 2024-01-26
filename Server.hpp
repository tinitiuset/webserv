#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include "defaults.hpp"
#include "Logger.hpp"

class Server {

private:
	int sockfd;
	sockaddr_in servaddr;

public:
	Server();
	~Server();

	int getSocketFd() const;
	int Accept() const;
};

#endif
