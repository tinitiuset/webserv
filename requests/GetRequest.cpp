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


	if (dynamic_cast<Redirect*>(conf->getServer(port).location(_uri)))
		return redirect();

	if (Index* loc = dynamic_cast<Index*>(conf->getServer(port).bestLocation(_uri)))
	{
		//realpath
		if (loc->file() == "" && loc->autoindex())
		{
			autoindex = true;
			root = loc->root();
			locat = loc->path();
		}
		std::cout << "\n*****uri: " << _uri << std::endl;
		std::cout << "*****location path: " << loc->path() << std::endl;
		std::cout << "*****root: " << loc->root() << std::endl;
		std::cout << "*****file: " << loc->file() << std::endl;
		std::cout << "*****autoindex: " << loc->autoindex() << std::endl;
		std::cout << "*****cgi: " << loc->cgi() << std::endl;
		resPath = loc->buildRealPath(_uri);
		std::cout << "*****realpath: " << resPath << std::endl << std::endl;
	}
	

	Logger::info("GetRequest::handle() handling GET request");

	Response response;

	//Resource resource("." + _uri);

	Resource resource("." + resPath);

	/* if (autoindex)
		response.set_body(resource.autoindex());  */
	if (autoindex)
		response.set_body(resource.buildAI(_uri, port, address, resPath));
	else
		response.set_body(resource.load());

	std::map<std::string, std::string> headers;
	headers.insert(std::make_pair("Content-Type", "text/html"));
	headers.insert(std::make_pair("Content-Length", Utils::toString(response.body().length())));


	if (!Cookie::isValidCookie(_headers))
		headers["Set-Cookie"] = Cookie::getSetCookieValue();

	response.set_headers(headers);
	response.set_start_line(resource.status());

	Logger::info("GetRequest::handle() returning response -> " + response.start_line());
	return response.format();
}
