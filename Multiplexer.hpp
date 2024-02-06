#ifndef MULTIPLEXER_HPP
#define MULTIPLEXER_HPP

#include <iostream>
#include <vector>
#include "defaults.hpp"
#include "Server.hpp"
#include "Logger.hpp"

class Multiplexer {

private:
	std::vector<int> serverFdVec;
	std::vector<int> clientFdVec;

public:
	static bool	_endserver;

	Multiplexer();
	~Multiplexer();

    void		run(const Server &);
	static void	signalHandler(int signal);

	size_t		getServerFdIdx(int fd) const;
	size_t		getClientFdIdx(int fd) const;
	void		setServerFdVec(std::vector<std::vector<int> >);
	
};

int	getMaxFd(std::vector<std::vector<int> > sockfd);

#endif
