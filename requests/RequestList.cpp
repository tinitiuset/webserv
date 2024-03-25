#include "RequestList.hpp"

#include <sys/ioctl.h>
#include <unistd.h>

RequestList::RequestList() {
}

RequestList::RequestList(const RequestList&requestLst) {
	_requests = requestLst._requests;
}

RequestList& RequestList::operator=(const RequestList&requestLst) {
	_requests = requestLst._requests;
	return *this;
}

RequestList::~RequestList() {
}

Request* RequestList::getRequest(const int& fd){
	if (_requests.find(fd) == _requests.end())
		return NULL;
	return _requests[fd];
}

void RequestList::addRequest(const int& fd, Request* request) {
	_requests[fd] = request;
}

void RequestList::removeRequest(const int& fd) {
	if (_requests.find(fd) != _requests.end())
	{
		delete _requests[fd];
		_requests.erase(fd);
	}
}
