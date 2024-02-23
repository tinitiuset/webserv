#include "GetRequest.hpp"
#include "../cookies/Cookie.hpp"

GetRequest::GetRequest(const Request& request): Request(request) {}

GetRequest::~GetRequest() {}

std::string GetRequest::handle() {

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

	if (!Cookie::isValidCookie(_headers))
		headers["Set-Cookie: "] = Cookie::getSetCookieValue();

	std::cout << "\nEN SET-COOKIE: \nheaders[Set-Cookie] = " << headers["Set-Cookie: "] << "\n" << std::endl;

	//volvemos a imprimir map headers
	std::cout << "\n^^^^^^Printing headers de la request" << std::endl;
	std::map<std::string, std::string>::const_iterator it;
	for (it = headers.begin(); it != headers.end(); ++it) {
		std::cout << it->first << " => " << it->second << std::endl;
	}

	response.set_headers(headers);
	response.set_start_line(resource.status());

	Logger::info("GetRequest::handle() returning response -> " + response.start_line());
	return response.format();
}
