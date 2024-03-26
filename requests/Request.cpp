#include "Request.hpp"

#include <sys/ioctl.h>
#include <unistd.h>

Request::Request(int &fd) : _fd(fd), _index(0), _isLonger(false) {
}

Request::Request(const Request&request) {
	_fd = request._fd;
	_index = request._index;
	_raw = request._raw;
	_method = request._method;
	_uri = request._uri;
	_headers = request._headers;
	_body = request._body;
	_isLonger = request._isLonger;
}

Request& Request::operator=(const Request&request) {
	_fd = request._fd;
	_index = request._index;
	_raw = request._raw;
	_method = request._method;
	_uri = request._uri;
	_headers = request._headers;
	_body = request._body;
	_isLonger = request._isLonger;
	return *this;
}

Request::~Request() {
}

ssize_t Request::read(int bf) {
	char buffer[bf];
	ssize_t bytesReceived;

	std::fill(buffer, buffer + sizeof(buffer), 0);
	bytesReceived = recv(_fd, buffer, sizeof(buffer), 0);
	if (bytesReceived == -1)
		throw std::runtime_error("recv failed");
	if (bytesReceived > 0)
		_raw.append(buffer, bytesReceived);
	return bytesReceived;
}

ssize_t Request::write() {
	ssize_t bytesSent = send(_fd, _raw.c_str(), _raw.size(), 0);
	if (bytesSent == -1)
		throw std::runtime_error("send failed");
	if (bytesSent > 0)
		_raw.erase(0, bytesSent);
	return bytesSent;
}

void Request::parseRequest() {

    std::istringstream requestStream(_raw);

    std::string requestLine;
    std::getline(requestStream, requestLine);
	
	Logger::debug(requestLine);

    std::istringstream requestLineStream(requestLine);

    requestLineStream >> _method >> _uri;

    std::string headerLine;

	try {
		while (std::getline(requestStream, headerLine) && headerLine != "\r") {
			headerLine.erase(headerLine.end() - 1, headerLine.end());
			std::istringstream headerLineStream(headerLine);
			std::string key;
			std::getline(headerLineStream, key, ':');
			std::string value;
			std::getline(headerLineStream, value);
			_headers[key] = value.substr(1);
		}
	} catch (const std::exception& e) {
	}
    _body = std::string(std::istreambuf_iterator<char>(requestStream), std::istreambuf_iterator<char>());

}

void Request::parseBody() {
	_body = _raw.substr(_raw.find("\r\n\r\n") + 4);
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

std::map<std::string, std::string>& Request::getHeaders() {
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

int Request::getFd() const {
	return _fd;
}

std::string Request::getUri() const {
	return _uri;
}

void Request::handle() {
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
	{
		std::cout << "servname: " <<  std::endl;
		throw RequestException(400);
	}
}

// Function checks if there is Content Length and if it is not bigger than the body_size.
// Gives 0 if its ok, -1 if not
int Request::checkContentLength() {
	if (_isLonger)
		return (-1);
	try{
		std::map<std::string, std::string>::const_iterator it = _headers.find("Content-Length");
		if(it == _headers.end())
			throw RequestException(411);
		else if (Utils::toInt(it->second) > conf->getServer(getPort()).body_size())
			throw RequestException(413);
		return (0);
	}
	catch (const RequestException&exception) {
		_isLonger = true;
		Response response;
		response.set_start_line("HTTP/1.1 " + Codes::status(exception.status()));
		Server server = conf->getServer(getPort());
		if (!server.errorPage(exception.status()).empty()) {
			Logger::info("Content-Length error loading error page from " + server.root() + server.errorPage(exception.status()));
			std::ifstream file((server.root() + "/" + server.errorPage(exception.status())).c_str());
			response.set_body(std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()));
		}
		else {
			Logger::debug("Content-Length error building default error page for status " + Utils::toString(exception.status()));
			response.set_body(ErrorPage::build(exception.status()));
		}
		std::map<std::string, std::string> headers;
		headers.insert(std::make_pair("Content-Type", "text/html"));
		headers.insert(std::make_pair("Content-Length", Utils::toString(response.body().length())));
		response.set_headers(headers);
		Logger::info("Content-Length error returning response -> " + response.start_line());
		_raw = response.format();
		return (-1);
	}
	
}
