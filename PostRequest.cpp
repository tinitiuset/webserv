#include "PostRequest.hpp"
#include <sstream>
#include <iostream>
#include <fstream>

PostRequest::PostRequest(const Request& request): Request(request) {
	parse_type();
	parse_body(_body);
}

PostRequest::~PostRequest() {}
std::string PostRequest::handle() {

	// AddToList
	// UploadFile
	// SetCookie
	// deChunck

	Logger::debug("PostRequest::handle() called");

	save_file(_body);

	Response response;

	response.set_body("File correctly uploaded!");

	std::map<std::string, std::string> headers;
	headers.insert(std::make_pair("Content-Type", "text/plain"));
	headers.insert(std::make_pair("Content-Length", std::to_string(response.body().length())));


	response.set_headers(headers);

	response.set_start_line("HTTP/1.1 200 OK");

	return response.format();
}

void PostRequest::parse_type(void) {
	std::string type = _headers["Content-Type"];
	std::istringstream contStrm(type);
	std::string value, key;

	std::getline(contStrm, value, ';');
	this->_headers["Content-Type"] = value.substr(1);
	while (std::getline(contStrm, key, '='))
	{
		std::getline(contStrm, value, ';');
		this->_headers[key.substr(1)] = value;
	}
}

void	PostRequest::parse_body(std::string body){
	std::string boundary = _headers["boundary"];
	std::string delimiter = "--" + boundary;
	std::string delimiter_end = "--" + boundary + "--";
	std::istringstream bodyStream(body);
	std::string line;
	std::string real_body;
	bool isNotFirst = false;

	std::getline(bodyStream, line);
	if (line == delimiter) {
		std::getline(bodyStream, line);
		while (line != delimiter_end && line != "")
		{
			std::istringstream lineStream(line);
			std::string key;
			std::string value;
			std::getline(lineStream, key, ':');
			std::getline(lineStream, value, ';');
			_postHeaders[key] = value.substr(1);
			while (std::getline(lineStream, key, '='))
			{
				std::getline(lineStream, value, ';');
				_postHeaders[key.substr(1)] = value.substr(1).substr(0, value.length() - 2);
			}
			std::getline(bodyStream, line);
		}
		while (std::getline(bodyStream, line) && line != delimiter_end)
		{
			if (isNotFirst)
				real_body += "\n";
			else
				isNotFirst = true;
			real_body += line;
			//std::getline(bodyStream, line);
		}
		_body = real_body;
	}
}

void	PostRequest::save_file(std::string body){
	std::cout << body << std::endl;
	std::string path = _uri + _postHeaders["filename"];
	std::ofstream outfile(path, std::ios::out | std::ios::binary);
	outfile << body << std::endl;

	outfile.close();

}