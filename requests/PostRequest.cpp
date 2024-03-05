#include "PostRequest.hpp"
#include <sstream>
#include <iostream>
#include <fstream>
#include "../cookies/Cookie.hpp"

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
	std::string resPath = Utils::extractFilePath(_uri);
	std::string qStr = _body;
	
	Logger::info("PostRequest::handle() handling Post request");

	Index* loc = dynamic_cast<Index*>(conf->getServer(getPort()).bestLocation(_uri));

	resPath = loc->buildRealPath(resPath);
	for (size_t i = 0; i < resPath.length() - 1; ++i) 
	{
		if (resPath[i] == '/' && resPath[i + 1] == '/') {
			resPath.erase(i, 1);
			--i;
		}
	}

	Response response;

	Resource resource(resPath, _method);

	if (loc->cgi() == true && (resPath.substr(resPath.length() - 3) == ".py" || resPath.substr(resPath.length() - 3) == ".pl"))
		response.set_body(resource.buildCGI(qStr));
	else
	{
		save_file(_body, loc->root() + "/" + _postHeaders["filename"]);

		response.set_body("File correctly uploaded!");
	}
	
	std::map<std::string, std::string> headers;
	if (!Cookie::isValidCookie(_headers))
	{
		std::string cookie = Cookie::getSetCookieValue();
		headers["Set-Cookie"] = "webserv = " + cookie;
		int fd = open("./cookies/cookies.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);	
		cookie += "\n";
		write(fd, cookie.c_str(), cookie.length());
		close(fd);		
	}

	headers.insert(std::make_pair("Content-Type", "text/plain"));
	headers.insert(std::make_pair("Content-Length", Utils::toString(response.body().length())));

	response.set_headers(headers);
	response.set_start_line("HTTP/1.1 200 OK");

	Logger::info("GetRequest::handle() returning response -> " + response.start_line());
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
void	PostRequest::save_file(std::string body, std::string path){
	
	//std::string path = loc->root() + "/" + _postHeaders["filename"];
	std::ofstream outfile(path, std::ios::out | std::ios::binary);

	outfile << body;
	outfile.flush();
	outfile.close();
}