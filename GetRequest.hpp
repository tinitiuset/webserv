#ifndef GETREQUEST_HPP
#define GETREQUEST_HPP
#include "Request.hpp"

#include "Response.hpp"

class GetRequest: public Request {

public:
	GetRequest(const Request&);
	~GetRequest();

	std::string handle();
};

#endif
