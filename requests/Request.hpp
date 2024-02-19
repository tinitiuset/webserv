#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <map>
#include <string>
#include "Response.hpp"
#include "../utils/defaults.hpp"
#include "../utils/Logger.hpp"

static std::string status(int code);

class Request {
public:

	Request();
	Request(const Request&);
	Request& operator=(const Request&);
	virtual ~Request();

	void parseRequest(const int &);
	void printRequest() const;
	bool isGetRequest() const;
	bool isPostRequest() const;

	virtual std::string handle() {return NULL;};
	std::string redirect();

protected:
	std::string _method;
	std::string _uri;
	std::map<std::string, std::string> _headers;
	std::string _body;
};

#endif