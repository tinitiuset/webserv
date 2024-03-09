#include "Codes.hpp"

std::string Codes::status(int status) {

	std::map<int, std::string> statusCodes;

	statusCodes[200] = "HTTP/1.1 200 OK";
	statusCodes[201] = "HTTP/1.1 201 Created";
	statusCodes[202] = "HTTP/1.1 202 Accepted";
	statusCodes[204] = "HTTP/1.1 204 No Content";
	statusCodes[301] = "HTTP/1.1 301 Moved Permanently";
	statusCodes[302] = "HTTP/1.1 302 Found";
	statusCodes[303] = "HTTP/1.1 303 See Other";
	statusCodes[304] = "HTTP/1.1 304 Not Modified";
	statusCodes[400] = "HTTP/1.1 400 Bad Request";
	statusCodes[401] = "HTTP/1.1 401 Unauthorized";
	statusCodes[403] = "HTTP/1.1 403 Forbidden";
	statusCodes[404] = "HTTP/1.1 404 Not Found";
	statusCodes[405] = "HTTP/1.1 405 Method Not Allowed";
	statusCodes[406] = "HTTP/1.1 406 Not Acceptable";
	statusCodes[408] = "HTTP/1.1 408 Request Timeout";
	statusCodes[409] = "HTTP/1.1 409 Conflict";
	statusCodes[411] = "HTTP/1.1 411 Length Required";
	statusCodes[413] = "HTTP/1.1 413 Payload Too Large";
	statusCodes[414] = "HTTP/1.1 414 URI Too Long";
	statusCodes[415] = "HTTP/1.1 415 Unsupported Media Type";
	statusCodes[500] = "HTTP/1.1 500 Internal Server Error";
	statusCodes[501] = "HTTP/1.1 501 Not Implemented";
	statusCodes[502] = "HTTP/1.1 502 Bad Gateway";
	statusCodes[503] = "HTTP/1.1 503 Service Unavailable";
	statusCodes[505] = "HTTP/1.1 505 HTTP Version Not Supported";

	return statusCodes[status];
}
