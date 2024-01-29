#ifndef MULTIPLEXER_HPP
#define MULTIPLEXER_HPP

#include <iostream>
#include "defaults.hpp"
#include "Server.hpp"
#include "Logger.hpp"

class Multiplexer {

public:
	Multiplexer();
	~Multiplexer();

    void run(const Server &);

};

int	getMaxFd(std::vector<std::vector<int> > sockfd);

#endif
