#include "GetRequest.hpp"

#include <string>

GetRequest::GetRequest(const Request& request): Request(request) {}

GetRequest::~GetRequest() {}

std::string GetRequest::handle() {

	Logger::info("GetRequest::handle() handling GET request");

	Response response;

	Resource resource("." + _uri);

	response.set_body(resource.load());

	std::map<std::string, std::string> headers;
	headers.insert(std::make_pair("Content-Type", "text/html"));
	headers.insert(std::make_pair("Content-Length", std::to_string(response.body().length())));

	response.set_headers(headers);
	response.set_start_line(resource.status());

	Logger::info("GetRequest::handle() returning response -> " + response.start_line());
	return response.format();
}