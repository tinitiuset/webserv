#include "GetRequest.hpp"

GetRequest::GetRequest(const Request&request): Request(request) {
}

GetRequest::~GetRequest() {
}

void GetRequest::handle() {
	Logger::info("GetRequest::handle() handling GET request");

	Response response;

	try {
		Request::hostnameAllowed();

		if (dynamic_cast<Redirect *>(conf->getServer(getPort()).bestLocation(_uri))) {
			_raw = redirect();
			return;
		}

		Request::methodAllowed();

		std::string resPath = Utils::extractFilePath(_uri);
		std::string qStr = Utils::extractQStr(_uri);
		int port = getPort();
		std::string address = conf->getServer(port).address();
		std::string host = Utils::removeLastSlash(getHost());

		Index* loc;

		if (!(loc = dynamic_cast<Index *>(conf->getServer(port).bestLocation(_uri))))
			throw RequestException(404);

		resPath = loc->buildRealPath(resPath);
		for (size_t i = 0; i < resPath.length() - 1; ++i) {
			if (resPath[i] == '/' && resPath[i + 1] == '/') {
				resPath.erase(i, 1);
				--i;
			}
		}

		Resource resource(resPath, _method);

		std::map<std::string, std::string> headers;

		if (loc && loc->file() == "" && loc->autoindex() && Utils::isDirectory(resPath.c_str())) {
			response.set_body(resource.buildAI(_uri, host, resPath));
			headers.insert(std::make_pair("Content-Type", "text/html"));
		}
		else if (loc && loc->cgi() == true && (resPath.substr(resPath.length() - 3) == ".py" || resPath.substr(
			                                       resPath.length() - 3) == ".pl"))
			response.set_body(resource.buildCGI(qStr));
		else {
			response.set_body(resource.load());
			headers.insert(std::make_pair("Content-Type", resource.mime()));
		}

		headers.insert(std::make_pair("Content-Length", Utils::toString(response.body().length())));

		if (!Cookie::isValidCookie(_headers)) {
			std::string cookie = Cookie::getSetCookieValue();
			headers["Set-Cookie"] = "webserv = " + cookie;
			int fd = open("./cookies/cookies.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
			cookie += "\n";
			::write(fd, cookie.c_str(), cookie.length());
			close(fd);
		}

		response.set_headers(headers);
		response.set_start_line(resource.status());
	}
	catch (const RequestException&exception) {
		response.set_start_line("HTTP/1.1 " + Codes::status(exception.status()));
		Server server = conf->getServer(getPort());
		if (!server.errorPage(exception.status()).empty()) {
			Logger::info("GetRequest::handle() loading error page from " + server.root() + server.errorPage(exception.status()));
			std::ifstream file((server.root() + "/" + server.errorPage(exception.status())).c_str());
			response.set_body(std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()));
		}
		else {
			Logger::debug("GetRequest::handle() building default error page for status " + Utils::toString(exception.status()));
			response.set_body(ErrorPage::build(exception.status()));
		}
		std::map<std::string, std::string> headers;
		headers.insert(std::make_pair("Content-Type", "text/html"));
		headers.insert(std::make_pair("Content-Length", Utils::toString(response.body().length())));
		response.set_headers(headers);
	}

	Logger::info("GetRequest::handle() returning response -> " + response.start_line());
	_raw = response.format();
}
