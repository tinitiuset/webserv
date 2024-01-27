#include "GetRequest.hpp"

#include <string>

GetRequest::GetRequest(const Request& request): Request(request) {}

GetRequest::~GetRequest() {}

std::string GetRequest::handle() {

	// Needs work
	// Needs work
	// Needs work

	Logger::debug("GetRequest::handle() called");

	Response response;

	response.set_body("<html><body><h1>Hello World</h1></body></html>");

	std::map<std::string, std::string> headers;
	headers.insert(std::make_pair("Content-Type", "text/html"));
	headers.insert(std::make_pair("Content-Length", std::to_string(response.body().length())));

	response.set_headers(headers);

	response.set_start_line("HTTP/1.1 200 OK");

	return response.format();
}
