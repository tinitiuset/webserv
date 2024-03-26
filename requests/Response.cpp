#include "Response.hpp"
#include <iostream>


Response::Response() {}

Response::Response(const Response& other) {
	*this = other;
}

Response& Response::operator=(const Response& other) {
	if (this != &other) {
		_start_line = other._start_line;
		_headers = other._headers;
		_body = other._body;
	}
	return *this;
}

Response::~Response() {}

std::string Response::start_line() const {
	return _start_line;
}

void Response::set_start_line(const std::string& start_line) {
	_start_line = start_line;
}

std::map<std::string, std::string> Response::headers() const {
	return _headers;
}

void Response::set_headers(const std::map<std::string, std::string>& headers) {
	_headers = headers;
}

std::string Response::body() const {
	return _body;
}

void Response::set_body(const std::string& body) {
	_body = body;
}

std::string Response::format() const {
	std::string formatted_response = _start_line + "\n";

	std::map<std::string, std::string>::const_iterator it;
	for (it = _headers.begin(); it != _headers.end(); ++it)
		formatted_response += it->first + ": " + it->second + "\n";


	formatted_response += "\n" + _body;

	return formatted_response;
}

