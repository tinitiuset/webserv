#include "Location.hpp"

Location::~Location() {}

std::string Location::path() const { return _path; }

Redirect::Redirect(std::string& loc) {
	_path = loc.substr(loc.find("location") + 9, loc.find('{') - loc.find("location") - 10);
	_code = std::stoi(loc.substr(loc.find("{redirect") + 10, loc.find(';', loc.find("{redirect")) - loc.find("{redirect") - 10));
	_redirect = loc.substr(loc.find_last_of(' ') + 1, loc.find(';', loc.find_last_of(' ')) - loc.find_last_of(' ') - 1);
}

Redirect::~Redirect() {}

int Redirect::code() const { return _code; }

std::string Redirect::redirect() const { return _redirect; }

Index::Index(std::string& loc) {
	_path = loc.substr(loc.find("location") + 9, loc.find('{') - loc.find("location") - 10);
	_file = loc.substr(loc.find("root") + 5, loc.find(';', loc.find("root")) - loc.find("root") - 5);
	_autoindex = loc.find("autoindex on") != std::string::npos;

	std::string methods = loc.substr(loc.find("methods") + 8, loc.find(';', loc.find("methods")) - loc.find("methods") - 8);
	std::stringstream ss(methods);
	std::string method;
	while (std::getline(ss, method, ' ')) {
		_methods.push_back(method);
	}
}

Index::~Index() {}

std::string Index::file() {	return _file; }

std::list<std::string> Index::methods() { return _methods; }
