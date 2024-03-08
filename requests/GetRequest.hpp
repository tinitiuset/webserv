#ifndef GETREQUEST_HPP
# define GETREQUEST_HPP

# include "Request.hpp"
# include "../utils/Logger.hpp"
# include "Response.hpp"
# include "Resource.hpp"

class GetRequest: public Request {

public:
	GetRequest(const Request&);
	~GetRequest();

	std::string handle();
};

#endif
