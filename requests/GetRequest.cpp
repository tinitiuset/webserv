#include "GetRequest.hpp"
#include "../cookies/Cookie.hpp"
#include "../utils/Utils.hpp"

GetRequest::GetRequest(const Request& request): Request(request) {}

GetRequest::~GetRequest() {}

std::string GetRequest::handle() {

	std::string resPath= _uri;
	std::string root = "";
	std::string locat = "";
	int			port = getPort();
	std::string address = conf->getServer(port).address();
	bool		autoindex = false;
	std::string host = Utils::removeLastSlash(getHost());

	if (dynamic_cast<Redirect*>(conf->getServer(port).location(_uri)))
		return redirect();

	if (Index* loc = dynamic_cast<Index*>(conf->getServer(port).bestLocation(_uri)))
	{
		if (loc->file() == "" && loc->autoindex())
		{
			autoindex = true;
			root = loc->root();
			locat = loc->path();
		}
		resPath = loc->buildRealPath(_uri);
		for (size_t i = 0; i < resPath.length() - 1; ++i) {
        	if (resPath[i] == '/' && resPath[i + 1] == '/') {
            	resPath.erase(i, 1);
            	--i;
        	}
    	}
	}
	
	Logger::info("GetRequest::handle() handling GET request");

	Response response;

	std::string dotPath = "." + resPath;
	Resource resource(dotPath);

	std::map<std::string, std::string> headers;

	if (autoindex && Utils::isDirectory(dotPath.c_str()))
	{
		response.set_body(resource.buildAI(_uri, host, resPath));
		headers.insert(std::make_pair("Content-Type", "text/html"));
	}
	else
	{
		response.set_body(resource.load());
		headers.insert(std::make_pair("Content-Type", resource.mime()));
	}

	headers.insert(std::make_pair("Content-Length", Utils::toString(response.body().length())));

	if (!Cookie::isValidCookie(_headers))
	{
		std::string cookie = Cookie::getSetCookieValue();
		headers["Set-Cookie"] = "webserv = " + cookie;
		int fd = open("./cookies/cookies.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);	
		cookie += "\n";
		write(fd, cookie.c_str(), cookie.length());
		close(fd);		
	}

	response.set_headers(headers);
	response.set_start_line(resource.status());

	Logger::info("GetRequest::handle() returning response -> " + response.start_line());
	return response.format();
}
