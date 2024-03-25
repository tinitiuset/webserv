#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <map>
# include <string>
# include <cstring>
# include "Response.hpp"
# include "../utils/defaults.hpp"
# include "../utils/Codes.hpp"
# include "../utils/Logger.hpp"
# include "../cookies/Cookie.hpp"
# include "../utils/Utils.hpp"
# include "../model/Location.hpp"
# include "../exceptions/RequestException.hpp"

class Request {
public:
	Request(const int&);
	Request(const Request&);
	Request& operator=(const Request&);
	virtual ~Request();

	void parseRequest();
	void read();
	void printRequest() const;

	bool isGetRequest() const;
	bool isPostRequest() const;
	bool isDeleteRequest() const;
	void methodAllowed() const;
	void hostnameAllowed() const;
	bool isReadComplete() const;
	int	getFd() const;

	virtual std::string handle();

	std::string getUri() const;
	std::map<std::string, std::string> getHeaders() const;

	int getPort() const;
	std::string getHost() const;

	std::string redirect() const;
	bool checkHostServName() const;

protected:
	Request();
	std::string _method;
	std::string _uri;
	std::map<std::string, std::string> _headers;
	std::string _body;
	std::string _request;
	bool		_read_complete;
	int			_fd;
};

#endif
