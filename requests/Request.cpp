#include "Request.hpp"

#include <sys/ioctl.h>
#include <unistd.h>

Request::Request() {
}

Request::Request(const Request&request) {
	_method = request._method;
	_uri = request._uri;
	_headers = request._headers;
	_body = request._body;
}

Request& Request::operator=(const Request&request) {
	_method = request._method;
	_uri = request._uri;
	_headers = request._headers;
	_body = request._body;
	return *this;
}

Request::~Request() {
}

bool Request::parseRequest(const int&fd) {
    char buffer[9999];
    std::string request;
    ssize_t bytesReceived;

	printf("-fd: %d\n", fd);

    do {
    	std::fill(buffer, buffer + sizeof(buffer), 0);
        bytesReceived = recv(fd, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived == -1) {
            throw std::runtime_error("recv failed");
            break;
        }
        request.append(buffer, bytesReceived);
    } while (bytesReceived == sizeof(buffer) - 1);




	Logger::debug("Raw request: " + request);

	if (request.find("GET") != 0 && request.find("POST") != 0 && request.find("DELETE") != 0)
		return false;

    std::istringstream requestStream(request);


    std::string requestLine;
    std::getline(requestStream, requestLine);
    std::istringstream requestLineStream(requestLine);

    requestLineStream >> _method >> _uri;

	

    std::string headerLine;
    while (std::getline(requestStream, headerLine) && headerLine != "\r"){
        headerLine.erase(headerLine.end() - 1, headerLine.end());
        std::istringstream headerLineStream(headerLine);
        std::string key;
        std::getline(headerLineStream, key, ':');
        std::string value;
        std::getline(headerLineStream, value);
		
        _headers[key] = value.substr(1);
    }


    if (_headers.find("Content-Length") != _headers.end()
    	&& _headers["User-Agent"].find("Mozilla") == std::string::npos) {
        int contentLength = Utils::toInt(_headers["Content-Length"]);

    	while (_body.size() < static_cast<unsigned long>(contentLength)) {
    		std::fill(buffer, buffer + sizeof(buffer), 0);
            bytesReceived = recv(fd, buffer, sizeof(buffer) - 1, 0);
    		if (bytesReceived == 0) {
    			break;
			}
            if (bytesReceived > 0)
            	_body.append(buffer, bytesReceived);
        }
    } else {
        _body = std::string(std::istreambuf_iterator<char>(requestStream), std::istreambuf_iterator<char>());
    }

	//imprimir la request sin el body
	//Logger::debug("Request: " + request.substr(0, request.find("\r\n\r\n")));


	
	return true;
}

int Request::getPort() const {
	std::map<std::string, std::string>::const_iterator it = _headers.find("Host");

	if (it != _headers.end()) {
		size_t pos = it->second.find(":");
		if (pos == std::string::npos)
			return (-1);
		return (std::atoi(it->second.substr(pos + 1).c_str()));
	}
	return (-1);
}

std::string Request::getHost() const {
	std::map<std::string, std::string>::const_iterator it = _headers.find("Host");

	if (it != _headers.end())
		return (it->second);
	return ("");
}

std::map<std::string, std::string> Request::getHeaders() const {
	return _headers;
}

bool Request::checkHostServName() const {
	if (getHost() == conf->getServer(getPort()).server_name())
		return true;
	return false;
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

std::string Request::redirect() const {
	Redirect* redirect = dynamic_cast<Redirect *>(conf->getServer(getPort()).bestLocation(_uri));

	Response response;

	response.set_start_line("HTTP/1.1 " + Codes::status(redirect->code()));
	std::map<std::string, std::string> headers;
	headers.insert(std::make_pair("Location", redirect->redirect()));
	response.set_headers(headers);

	Logger::debug("Request::redirect() returning response -> " + response.format());

	return response.format();
}

std::string Request::getUri() const {
	return _uri;
}

std::string Request::handle() {
	return NULL;
}

void Request::methodAllowed() const {
	Index* loc = dynamic_cast<Index *>(conf->getServer(getPort()).bestLocation(_uri));

	if (this->isGetRequest() && loc->isMethodAllowed("get") == false)
		throw RequestException(405);
	if (this->isPostRequest() && loc->isMethodAllowed("post") == false)
		throw RequestException(405);
	if (this->isDeleteRequest() && loc->isMethodAllowed("delete") == false)
		throw RequestException(405);
}

void Request::hostnameAllowed() const {
	Server server = conf->getServer(getPort());
	if (std::string(server.server_name() + ":" + Utils::toString(server.port())) != getHost())
		throw RequestException(400);
}
