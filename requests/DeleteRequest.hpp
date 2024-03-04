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
		int	delete_file(std::string path);
		int	delete_directory(std::string path);
};

#endif