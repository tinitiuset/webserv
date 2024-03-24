#ifndef POSTREQUEST_HPP
# define POSTREQUEST_HPP

# include "Request.hpp"
# include "Response.hpp"
# include "Resource.hpp"


class PostRequest: public Request {
	private:
		std::map<std::string, std::string> _postHeaders;
	public:
		PostRequest(const Request&);
		~PostRequest();
		void		handle();
		void		parse_multipart_body(std::string body);
		void		parse_type();
		void		save_file(std::string body);
};

#endif
