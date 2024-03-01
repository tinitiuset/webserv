#include "Request.hpp"
#include "../utils/Utils.hpp"

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
	char buffer[10024] = {0};
	read(fd, buffer, 10024);
	std::string request(buffer);

	Logger::debug("Raw request: " + request);

	std::istringstream requestStream(request);

	std::string requestLine;
	std::getline(requestStream, requestLine);
	std::istringstream requestLineStream(requestLine);

	requestLineStream >> _method >> _uri;

	std::string headerLine;
	while (std::getline(requestStream, headerLine) && headerLine != "\r") {
		headerLine.erase(std::remove(headerLine.end() - 1, headerLine.end(), '\r'), headerLine.end()); // Remove trailing '\r'
		std::istringstream headerLineStream(headerLine);
		std::string key;
		std::getline(headerLineStream, key, ':');
		std::string value;
		std::getline(headerLineStream, value);
		_headers[key] = value.substr(1);
	}
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
