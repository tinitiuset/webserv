#include "ErrorPage.hpp"

std::string ErrorPage::build(int status) {
	std::string code = Codes::status(status);
	std::string page = "<!doctype html>\n";
	page += "<html>\n";
	page += "<head>\n";
	page += "<title>" +  code.substr(0,3) + " " + code.substr(4) + "</title>\n";
	page += "<style>\n";
	page += "html, body { height: 100%; margin: 0; padding: 0; }\n";
	page += "body { background-color: #282a36; color: #f8f8f2; font-family: Arial, sans-serif; display: flex; flex-direction: column; justify-content: center; align-items: center; }\n";
	page += "h1 { font-size: 3em; font-weight: bold; }\n";
	page += "p { font-size: 2em; font-weight: bold; }\n";
	page += "</style>\n";
	page += "</head>\n";
	page += "<body>\n";
	page += "<h1>" + code.substr(0,3) + "</h1>\n";
	page += "<p>" + code.substr(4) + "</p>\n";
	page += "</body>\n";
	page += "</html>\n";
	return page;
}