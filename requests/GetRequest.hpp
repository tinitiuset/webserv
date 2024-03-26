#ifndef GETREQUEST_HPP
# define GETREQUEST_HPP

# include "Request.hpp"
# include "Resource.hpp"

class GetRequest : public Request {
public:
	GetRequest(const Request&);
	~GetRequest();

	void handle();
};

#endif
