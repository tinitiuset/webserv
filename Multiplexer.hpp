#ifndef MULTIPLEXER_HPP
#define MULTIPLEXER_HPP

#include <iostream>
#include "defaults.hpp"
#include "Server.hpp"
#include "Logger.hpp"

class Multiplexer {

private:

public:
	static bool	_endserver;

	Multiplexer();
	~Multiplexer();

    void	run(const Server &);
	static void	signalHandler(int signal);

};

int	getMaxFd(std::vector<std::vector<int> > sockfd);

#endif
