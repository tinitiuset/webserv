#include "Request.hpp"
#include "../utils/Utils.hpp"
#include <sstream>
#include <unistd.h>

static std::string status(int code) {
	switch (code) {
		case 301:
			return("Moved Permanently");
		break;
		case 302:
			return("Found");
		break;
		default:
			return("OK");
	}
}

Request::Request() {}

Request::Request(const Request& request) {
	_method = request._method;
	_uri = request._uri;
	_headers = request._headers;
	_body = request._body;
	//_location = request._location;
}

Request::Request(const int &fd, const std::list <Location*> locations) {
	this->parseRequest(fd);
	//this->searchLocation(_uri, locations);
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
	char buffer[99999] = {0};
	read(fd, buffer, 99999);
	std::string request(buffer);
	std::cout << request.size() << "----" << request.length() << std::endl;
	Logger::debug("Raw request: " + request);

	std::istringstream requestStream(request);

	std::string requestLine;
	std::getline(requestStream, requestLine);
	std::istringstream requestLineStream(requestLine);

	requestLineStream >> _method >> _uri;

	std::string headerLine;
	while (std::getline(requestStream, headerLine) && headerLine != "\r") {
		headerLine.erase(headerLine.end() - 1, headerLine.end()); // Remove trailing '\r'
		std::istringstream headerLineStream(headerLine);
		std::string key;
		std::getline(headerLineStream, key, ':');
		std::string value;
		std::getline(headerLineStream, value);
		_headers[key] = value.substr(1);
	}
	!_headers["Content-Length"].empty() ?
		_body = std::string(&buffer[requestStream.tellg()], Utils::toInt(_headers["Content-Length"])) : _body = "";
}



int Request::getPort() const
{
	std::map<std::string, std::string>::const_iterator it = _headers.find("Host");

	if (it != _headers.end())
	{
		size_t pos = it->second.find(":");
		if ( pos == std::string::npos)
			return (-1);
		else
			return(std::atoi(it->second.substr(pos + 1).c_str()));
	}
	else
		return (-1);
}

std::string Request::getHost() const
{
	std::map<std::string, std::string>::const_iterator it = _headers.find("Host");

	if (it != _headers.end())
		return (it->second);
	else
		return ("");
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

bool Request::isDeleteRequest() const {
	return _method == "DELETE";
}

std::string Request::redirect() {
	Redirect* redirect = dynamic_cast<Redirect*>(conf->getServer(getPort()).location(_uri));

	Response response;

	std::string startLine = "HTTP/1.1 " + Utils::toString(redirect->code()) + " " + status(redirect->code());

	response.set_start_line(startLine);
	std::map<std::string, std::string> headers;
	headers.insert(std::make_pair("Location", redirect->redirect()));
	response.set_headers(headers);

	Logger::debug("Request::redirect() returning response -> " + response.format());

	return response.format();
}


std::string Request::getUri() const {
	return _uri;
}