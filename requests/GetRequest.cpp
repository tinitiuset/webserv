#include "GetRequest.hpp"

GetRequest::GetRequest(const Request& request): Request(request) {}

GetRequest::~GetRequest() {}

std::string GetRequest::handle() {

	std::string resPath= _uri;

	if (dynamic_cast<Redirect*>(conf->getServer(getPort()).location(_uri)))
		return redirect();

	if (Index* loc = dynamic_cast<Index*>(conf->getServer(getPort()).bestLocation(_uri)))
	{
		//realpath
		std::cout << "\n*****uri: " << _uri << std::endl;
		std::cout << "*****location path: " << loc->path() << std::endl;
		std::cout << "*****root: " << loc->root() << std::endl;
		std::cout << "*****file: " << loc->file() << std::endl;
		resPath = loc->buildRealPath(_uri);
		std::cout << "*****realpath: " << resPath << std::endl << std::endl;
	}
	

	Logger::info("GetRequest::handle() handling GET request");

	Response response;

	//Resource resource("." + _uri);
	Resource resource("." + resPath);

	response.set_body(resource.load());

	std::map<std::string, std::string> headers;
	headers.insert(std::make_pair("Content-Type", "text/html"));
	headers.insert(std::make_pair("Content-Length", std::to_string(response.body().length())));

	response.set_headers(headers);
	response.set_start_line(resource.status());

	Logger::info("GetRequest::handle() returning response -> " + response.start_line());
	return response.format();
}
