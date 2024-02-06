#include "PostRequest.hpp"

PostRequest::PostRequest(const Request& request): Request(request), _type(_headers["Content-Type"]) {}

PostRequest::~PostRequest() {}
std::string PostRequest::handle() {

	// AddToList
	// UploadFile
	// SetCookie
	// deChunck

	Logger::debug("PostRequest::handle() called");

	

	Response response;

	response.set_body("File correctly uploaded!");

	std::map<std::string, std::string> headers;
	headers.insert(std::make_pair("Content-Type", "text/plain"));
	headers.insert(std::make_pair("Content-Length", std::to_string(response.body().length())));


	response.set_headers(headers);

	response.set_start_line("HTTP/1.1 200 OK");

	return response.format();
}