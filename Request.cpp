#include "Request.hpp"

#include <sstream>
#include <unistd.h>

Request::Request() {}

Request::Request(const Request& request) {
	_method = request._method;
	_uri = request._uri;
	_headers = request._headers;
	_body = request._body;
}

Request::Request(const int &fd, const std::vector<Location> &locations) {
	this->parseRequest(fd);
	this->searchLocation(_uri, locations);
}

Request& Request::operator=(const Request& request) {
	_method = request._method;
	_uri = request._uri;
	_headers = request._headers;
	_body = request._body;
	return *this;
}

Request::~Request() {}

void Request::parseRequest(const int &fd) {
	char buffer[1024] = {0};
	read(fd, buffer, 1024);
	std::string request(buffer);

	Logger::debug("Raw request: " + request);

	std::istringstream requestStream(request);

	std::string requestLine;
	std::getline(requestStream, requestLine);
	std::istringstream requestLineStream(requestLine);

	requestLineStream >> _method >> _uri;

	std::string headerLine;
	while (std::getline(requestStream, headerLine) && headerLine != "") {
		//headerLine.erase(headerLine.end() - 1, headerLine.end()); // Remove trailing '\r'
		std::istringstream headerLineStream(headerLine);
		std::string key;
		std::getline(headerLineStream, key, ':');
		std::string value;
		std::getline(headerLineStream, value);
		_headers[key] = value.substr(1);
	}

	std::getline(requestStream, _body, '\0');
}

void Request::printRequest() const {
	Logger::debug("Method: " + _method);
	Logger::debug("URI: " + _uri);
	Logger::debug("Headers:");
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it) {
		Logger::debug("\t" + it->first + ": " + it->second);
	}
	Logger::debug("Body: " + _body);
}

bool Request::isGetRequest() const {
	return _method == "GET";
}

bool Request::isPostRequest() const {
	return _method == "POST";
}

void Request::searchLocation(const std::string &path, const std::vector<Location> &locations) {
	std::size_t longestMatch = 0;
	for (std::vector<Location>::const_iterator it = locations.begin(); it != locations.end(); ++it) {
		if ((*it).getPath().length() > longestMatch)
		{
			if (path.find((*it).getPath()) == 0)
			{
				_location = *it;
				longestMatch = (*it).getPath().length();
			}
		}
	}
}

std::string Request::getUri() const {
	return _uri;
}