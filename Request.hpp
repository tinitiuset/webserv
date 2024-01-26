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
	~Request();

	void parseRequest(const int &);
	void printRequest() const;
	bool isGetRequest() const;
	bool isPostRequest() const;

private:
	std::string _method;
	std::string _uri;
	std::map<std::string, std::string> _headers;
	std::string _body;
};

#endif