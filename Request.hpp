#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <map>
#include <string>
#include <vector>
#include "Logger.hpp"
#include "Location.hpp"

/* class VServer{
	private:
		std::vector<Location> _locations;
}; */



class Request {
public:

	Request();
	Request(const Request&);
	Request(const int &fd, const std::vector<Location> &locations);
	Request& operator=(const Request&);
	virtual ~Request();

	void parseRequest(const int &);
	void printRequest() const;
	bool isGetRequest() const;
	bool isPostRequest() const;
	virtual std::string handle() {return NULL;};
	std::string getUri() const;

protected:
	void searchLocation(const std::string &path, const std::vector<Location> &locations);
	std::string _method;
	std::string _uri;
	std::map<std::string, std::string> _headers;
	std::string _body;
	Location _location;
};

#endif