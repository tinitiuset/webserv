#ifndef MULTIPLEXER_HPP
#define MULTIPLEXER_HPP

#include <iostream>
#include <vector>
#include "utils/defaults.hpp"
#include "utils/Logger.hpp"
#include "utils/Utils.hpp"

class Multiplexer {

private:
	std::vector<int> serverFdVec;
	std::vector<int> clientFdVec;

public:
	static bool	_endserver;

	Multiplexer();
	~Multiplexer();

    void		run();
	static void	signalHandler(int signal);

	size_t		getServerFdIdx(int fd) const;
	size_t		getClientFdIdx(int fd) const;
	void		setServerFdVec(std::vector<int>);
	
};

#endif
