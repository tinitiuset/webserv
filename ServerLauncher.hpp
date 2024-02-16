#ifndef SERVERLAUNCHER_HPP
#define SERVERLAUNCHER_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include "utils/defaults.hpp"
#include "utils/Logger.hpp"

class ServerLauncher {

private:
	std::vector<int> sockfd;
	//sockaddr_in servaddr;

public:
	ServerLauncher();
	~ServerLauncher();

	std::vector<int> getSocketFd() const;
	int 	Accept(size_t i) const;
	void	socketOps(int port, int i);
	std::vector<int> getSockFdCoords(int fd) const;

};

void	checkRepeatedPorts(const std::vector<int> &vector);

#endif
