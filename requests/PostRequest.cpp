#include "PostRequest.hpp"
#include <sstream>
#include <iostream>
#include <fstream>

PostRequest::PostRequest(const Request& request): Request(request) {
	parse_type();
	if (_headers["Content-Type"].find("multipart/form-data") != std::string::npos)
		parse_multipart_body(_body);
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

//Method to parse the type of the request if necessary
void PostRequest::parse_type(void) {
	std::string type = _headers["Content-Type"];
	std::istringstream contStrm(type);
	std::string value, key;

	std::getline(contStrm, value, ';');
	this->_headers["Content-Type"] = value;
	while (std::getline(contStrm, key, '='))
	{
		std::getline(contStrm, value, ';');
		this->_headers[key.substr(1)] = value;
	}
}

//Method to parse the body of the request when Content-Type is multipart/form-data
void	PostRequest::parse_multipart_body(std::string body){
	std::string boundary = _headers["boundary"];
	std::string delimiter = "--" + boundary;
	std::string delimiter_end = "--" + boundary + "--";
	std::istringstream bodyStream(body);
	std::string line;

	std::getline(bodyStream, line);
	line.erase(line.end() - 1, line.end()); // Remove trailing '\r'
	if (line == delimiter) {
		std::getline(bodyStream, line);
		line.erase(line.end() - 1, line.end()); // Remove trailing '\r'
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
			line.erase(line.end() - 1, line.end()); // Remove trailing '\r'
		}
		int const start = bodyStream.tellg();
        int const end = body.find(delimiter_end, start);

        // Extract the image data
        std::string const image_data = body.substr(start, end - start);

		_body = image_data;
	}
}

//Method to save the file in the server
void	PostRequest::save_file(std::string body){
	Index* loc = dynamic_cast<Index*>(conf->getServer(getPort()).bestLocation(_uri));
	std::string path = loc->root() + "/" + _postHeaders["filename"];
	std::ofstream outfile(path, std::ios::out | std::ios::binary);

	outfile << body;
	outfile.flush();
	outfile.close();
}