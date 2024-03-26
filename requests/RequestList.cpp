#include "RequestList.hpp"

RequestList::RequestList() {
}

RequestList::~RequestList() {
	for (std::vector<Request*>::iterator it = _requests.begin(); it != _requests.end(); ++it)
		delete *it;
	_requests.clear();
}

void RequestList::addRequest(Request* request) {
	_requests.push_back(request);
}

void RequestList::removeRequest(int fd) {
	for (std::vector<Request*>::iterator it = _requests.begin(); it != _requests.end(); ++it) {
		if ((*it)->getFd() == fd) {
			delete *it;
			_requests.erase(it);
			break;
		}
	}
}

Request* RequestList::getRequest(int fd) {
	for (std::vector<Request*>::iterator it = _requests.begin(); it != _requests.end(); ++it) {
		if ((*it)->getFd() == fd) {
			return *it;
		}
	}
	return NULL;
}
