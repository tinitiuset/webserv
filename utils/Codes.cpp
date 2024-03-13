#include "Codes.hpp"

std::string Codes::status(int status) {

	std::map<int, std::string> statusCodes;

	statusCodes[200] = "200 OK";
	statusCodes[201] = "201 Created";
	statusCodes[202] = "202 Accepted";
	statusCodes[204] = "204 No Content";
	statusCodes[301] = "301 Moved Permanently";
	statusCodes[302] = "302 Found";
	statusCodes[303] = "303 See Other";
	statusCodes[304] = "304 Not Modified";
	statusCodes[400] = "400 Bad Request";
	statusCodes[401] = "401 Unauthorized";
	statusCodes[403] = "403 Forbidden";
	statusCodes[404] = "404 Not Found";
	statusCodes[405] = "405 Method Not Allowed";
	statusCodes[406] = "406 Not Acceptable";
	statusCodes[408] = "408 Request Timeout";
	statusCodes[409] = "409 Conflict";
	statusCodes[411] = "411 Length Required";
	statusCodes[413] = "413 Payload Too Large";
	statusCodes[414] = "414 URI Too Long";
	statusCodes[415] = "415 Unsupported Media Type";
	statusCodes[500] = "500 Internal Server Error";
	statusCodes[501] = "501 Not Implemented";
	statusCodes[502] = "502 Bad Gateway";
	statusCodes[503] = "503 Service Unavailable";
	statusCodes[505] = "505 HTTP Version Not Supported";

	return statusCodes[status];
}
