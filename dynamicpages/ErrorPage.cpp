#include "ErrorPage.hpp"

std::string ErrorPage::build(int status, std::string message) {
	std::string page = "<!doctype html>\n";
	page += "<html>\n";
	page += "<head>\n";
	page += "<title>" + Utils::toString(status) + " " + message + "</title>\n";
	page += "<style>\n";
	page += "html, body { height: 100%; margin: 0; padding: 0; }\n";
	page += "body { background-color: #282a36; color: #f8f8f2; font-family: Arial, sans-serif; display: flex; flex-direction: column; justify-content: center; align-items: center; }\n";
	page += "h1 { font-size: 3em; font-weight: bold; }\n";
	page += "p { font-size: 2em; font-weight: bold; }\n";
	page += "</style>\n";
	page += "</head>\n";
	page += "<body>\n";
	page += "<h1>" + Utils::toString(status) + "</h1>\n";
	page += "<p>" + message + "</p>\n";
	page += "</body>\n";
	page += "</html>\n";
	return page;
}