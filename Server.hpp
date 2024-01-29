#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include "defaults.hpp"
#include "Logger.hpp"

class Server {

private:
	std::vector<std::vector<int> > sockfd;
	sockaddr_in servaddr;

public:
	Server();
	~Server();

	std::vector<std::vector<int> > getSocketFd() const;
	int Accept() const;
	void	socketOps(int port, int i, int j);
};

void	checkRepeatedPorts(const std::vector<int> &vector);

#endif
