#include "GetRequest.hpp"
#include "../cookies/Cookie.hpp"
#include "../utils/Utils.hpp"

GetRequest::GetRequest(const Request& request): Request(request) {}

GetRequest::~GetRequest() {}

std::string GetRequest::handle()
{
	std::string resPath = Utils::extractFilePath(_uri);
	std::string qStr = Utils::extractQStr(_uri);
	int			port = getPort();
	std::string address = conf->getServer(port).address();
	std::string host = Utils::removeLastSlash(getHost());

	Index*		loc;

	Logger::info("GetRequest::handle() handling GET request");

	if (dynamic_cast<Redirect*>(conf->getServer(port).location(_uri)))
		return redirect();

	if (!(loc = dynamic_cast<Index*>(conf->getServer(port).bestLocation(_uri))))
	{
		//return (Response::notFound().format());
		return ("");
	}

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

	std::map<std::string, std::string> headers;

	if (loc->file() == "" && loc->autoindex() && Utils::isDirectory(resPath.c_str()))
	{
		response.set_body(resource.buildAI(_uri, host, resPath));
		headers.insert(std::make_pair("Content-Type", "text/html"));
	}
	else if (loc->cgi() == true && (resPath.substr(resPath.length() - 3) == ".py" || resPath.substr(resPath.length() - 3) == ".pl"))
		response.set_body(resource.buildCGI(qStr));
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
