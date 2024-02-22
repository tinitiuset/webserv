#include "GetRequest.hpp"

GetRequest::GetRequest(const Request& request): Request(request) {}

GetRequest::~GetRequest() {}

std::string GetRequest::handle() {

	std::cout << "\n**  GetRequest::handle() handling GET request" << std::endl;
	std::cout << "**  GetRequest::handle() _uri: " << _uri << std::endl;
	std::cout << "**  GetRequest::handle() _method: " << _method << std::endl;
	//print headers map
	for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it)
		std::cout << "**  GetRequest::handle() _headers: " << it->first << " => " << it->second << std::endl;

	std::cout << "**  GetRequest::handle() _body: " << _body << std::endl << std::endl;

	std::cout << "----------------------" << conf->getServer(getPort()).port() << std::endl;

/* 	if (dynamic_cast<Redirect*>(conf->getServer(getPort()).location(_uri)))
		return redirect(); */

	if (dynamic_cast<Redirect*>(getServerInst().location(_uri)))
		return redirect();


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
