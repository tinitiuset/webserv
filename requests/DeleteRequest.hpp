#ifndef DELETEREQUEST_HPP
#define DELETEREQUEST_HPP

#include "Request.hpp"
#include "Response.hpp"
#include <cstdio>

class DeleteRequest: public Request {
	private:
	public:
		DeleteRequest(const Request&);
		~DeleteRequest();
		std::string handle();
		void	delete_file(std::string path);
};

#endif