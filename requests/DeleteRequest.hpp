#ifndef DELETEREQUEST_HPP
# define DELETEREQUEST_HPP

# include "Request.hpp"
# include "Response.hpp"
# include "Resource.hpp"
# include <cstdio>
# include <unistd.h>

class DeleteRequest: public Request {
	private:
	public:
		DeleteRequest(const Request&);
		~DeleteRequest();
		void	handle();
		void	delete_file(std::string path);
		void	delete_directory(std::string path);
};

#endif