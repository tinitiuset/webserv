#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <map>
#include <string>
#include "Logger.hpp"

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

protected:
	std::string _method;
	std::string _uri;
	std::map<std::string, std::string> _headers;
	std::string _body;
};

#endif