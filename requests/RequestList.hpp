#ifndef REQUESTLIST_HPP
#define REQUESTLIST_HPP

#include <vector>

#include "Request.hpp"

class RequestList {

public:
	RequestList();
	~RequestList();

	void addRequest(Request* request);
	void removeRequest(int fd);

	Request* getRequest(int fd);

private:
	std::vector<Request*> _requests;
};

#endif