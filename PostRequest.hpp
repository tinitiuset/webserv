#ifndef POSTREQUEST_HPP
#define POSTREQUEST_HPP

#include "Request.hpp"
#include "Response.hpp"

class PostRequest: public Request {
	private:
		std::string _real_body;
	public:
		PostRequest(const Request&);
		~PostRequest();
		std::string handle();
		void	parse_body(std::string body);
};

#endif
