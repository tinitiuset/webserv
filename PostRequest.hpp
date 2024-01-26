#ifndef POSTREQUEST_HPP
#define POSTREQUEST_HPP

#include "Request.hpp"

class PostRequest: public Request {

public:
	PostRequest(const Request&);
	~PostRequest();

};

#endif
