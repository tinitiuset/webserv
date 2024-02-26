#include "GetRequest.hpp"

GetRequest::GetRequest(const Request& request): Request(request) {}

GetRequest::~GetRequest() {}

std::string GetRequest::handle() {

/* 	if (dynamic_cast<Redirect*>(conf->getServer(getPort()).location(_uri)))
		return redirect(); */

	if (dynamic_cast<Redirect*>(getServerInst().location(_uri)))
		return redirect();

    //if (Location* loc = dynamic_cast<Index*>(conf->server(0).bestLocation(_uri)))
	//	return "";

	if (Index* loc = dynamic_cast<Index*>(getServerInst().bestLocation(_uri)))
	{
		std::cout << "GetRequest::handle() location found: _uri [" << _uri << "]" << std::endl;
		std::cout << "GetRequest::handle() location found: loc->index() [" << loc->index() << "]" << std::endl;
		std::cout << "GetRequest::handle() location found: loc->autoindex() [" << loc->autoindex() << "]" << std::endl;
		std::cout << "GetRequest::handle() location found: loc->cgi() [" << loc->cgi() << "]" << std::endl;
		std::cout << "GetRequest::handle() location found: loc->file() [" << loc->file() << "]\n" << std::endl;
		//return "";
	}

	Logger::info("GetRequest::handle() handling GET request");

	Response response;

	Resource resource("." + _uri);

	response.set_body(resource.load());

	std::map<std::string, std::string> headers;
	headers.insert(std::make_pair("Content-Type", "text/html"));
	headers.insert(std::make_pair("Content-Length", std::to_string(response.body().length())));

	response.set_headers(headers);
	response.set_start_line(resource.status());

	Logger::info("GetRequest::handle() returning response -> " + response.start_line());
	return response.format();
}
