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
# include "../dynamicpages/ErrorPage.hpp"

class Request {
public:
	Request(int &);
	Request(const Request&);
	Request& operator=(const Request&);
	virtual ~Request();

	ssize_t read(int);
	ssize_t write();

	void parseRequest();
	void parseBody();
	void printRequest() const;

	bool isGetRequest() const;
	bool isPostRequest() const;
	bool isDeleteRequest() const;
	void methodAllowed() const;
	void hostnameAllowed() const;

	virtual void handle();

	int getFd() const;
	std::string getUri() const;
	std::map<std::string, std::string>& getHeaders();

	int getPort() const;
	std::string getHost() const;

	std::string redirect() const;
	bool checkHostServName() const;

	int checkContentLength();

protected:
	int _fd;
	std::string _raw;
	std::string _method;
	std::string _uri;
	std::map<std::string, std::string> _headers;
	std::string _body;
	int _index;
	bool _isLonger;
};

#endif
