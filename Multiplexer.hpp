#ifndef MULTIPLEXER_HPP
#define MULTIPLEXER_HPP

#include <iostream>
#include <string>
#include <vector>

#include "utils/defaults.hpp"
#include "utils/Logger.hpp"
#include "utils/Utils.hpp"

#include "requests/Request.hpp"
#include "requests/RequestList.hpp"
#include "requests/GetRequest.hpp"
#include "requests/PostRequest.hpp"
#include "requests/DeleteRequest.hpp"

class Multiplexer {
private:
	std::vector<int> serverFdVec;
	std::vector<int> clientFdVec;

public:
	static bool _endserver;

	Multiplexer();
	~Multiplexer();

	void run();

	static void signalHandler(int signal);

	size_t getServerFdIdx(int fd) const;
	size_t getClientFdIdx(int fd) const;

	void setServerFdVec(const std::vector<int>&);
};

#endif
