#include "PostRequest.hpp"
#include <sstream>
#include <iostream>
#include <fstream>
#include "../cookies/Cookie.hpp"

PostRequest::PostRequest(const Request& request): Request(request) {
	parse_type();
}

PostRequest::~PostRequest() {}


void PostRequest::handle() {

	Response response;

	try {
		Request::hostnameAllowed();

		if (dynamic_cast<Redirect *>(conf->getServer(getPort()).bestLocation(_uri))) {
			_raw = redirect();
			return;
		}

		Request::methodAllowed();

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

		Resource resource(resPath, _method);

		if (loc->cgi() == true && (resPath.substr(resPath.length() - 3) == ".py" || resPath.substr(resPath.length() - 3) == ".pl"))
			response.set_body(resource.buildCGI(qStr));
		else
		{
			if(_headers["Content-Length"].empty())
				throw RequestException(411);
			else if (Utils::toInt(_headers["Content-Length"]) > conf->getServer(getPort()).body_size())
				throw RequestException(413);
			if (_headers["Content-Type"].find("multipart/form-data") != std::string::npos && !_body.empty())
				parse_multipart_body(_body);
			else
			{
				std::cout << "POAR" << std::endl;
				throw RequestException(400);
			}

			save_file(_body);

			response.set_body("File correctly uploaded!");
		}
		std::map<std::string, std::string> headers;
		if (!Cookie::isValidCookie(_headers))
		{
			std::string cookie = Cookie::getSetCookieValue();
			headers["Set-Cookie"] = "webserv = " + cookie;
			int fd = open("./cookies/cookies.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);	
			cookie += "\n";
			::write(fd, cookie.c_str(), cookie.length());
			close(fd);		
		}

		headers.insert(std::make_pair("Content-Type", "text/plain"));
		headers.insert(std::make_pair("Content-Length", Utils::toString(response.body().length())));

		response.set_headers(headers);
		response.set_start_line("HTTP/1.1 200 OK");

	}
	catch (const RequestException&exception) {
		response.set_start_line("HTTP/1.1 " + Codes::status(exception.status()));
		Server server = conf->getServer(getPort());
		if (!server.errorPage(exception.status()).empty()) {
			Logger::info("PostRequest::handle() loading error page from " + server.root() + server.errorPage(exception.status()));
			std::ifstream file((server.root() + "/" + server.errorPage(exception.status())).c_str());
			response.set_body(std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()));
		}
		else {
			Logger::debug("PostRequest::handle() building default error page for status " + Utils::toString(exception.status()));
			response.set_body(ErrorPage::build(exception.status()));
		}
		std::map<std::string, std::string> headers;
		headers.insert(std::make_pair("Content-Type", "text/html"));
		headers.insert(std::make_pair("Content-Length", Utils::toString(response.body().length())));
		response.set_headers(headers);
	}
	
	Logger::info("PostRequest::handle() returning response -> " + response.start_line());
	_raw = response.format();
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
	std::string path = Utils::strReplace(_uri, loc->path(), loc->root()) + "/" + _postHeaders["filename"];
	std::ofstream outfile(path.c_str(), std::ios::out | std::ios::binary);

	std::cout << "path: " << path << std::endl;
	if (!outfile.is_open())
	{
		outfile.close();
		std::cout << "**403 open" << std::endl;
		throw RequestException(403);
	}
	outfile << body;
	outfile.flush();
	outfile.close();
}