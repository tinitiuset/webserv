#include "Location.hpp"

Location::~Location() {}

std::string Location::path() const { return _path; }

std::string buildRealPath(std::string uri, std::string location, std::string root, std::string &file)
{
	location = Utils::removeLastSlash(location);
	uri = Utils::removeLastSlash(uri);
	root = Utils::removeLastSlash(root);

	if (uri == location)
		return (root + "/" + file);
	return (Utils::strReplace(uri, location, root));
}

Redirect::Redirect(std::string& loc) {
	_path = loc.substr(loc.find("location") + 9, loc.find('{') - loc.find("location") - 10);
	_code = std::stoi(loc.substr(loc.find("{redirect") + 10, loc.find(';', loc.find("{redirect")) - loc.find("{redirect") - 10));
	_redirect = loc.substr(loc.find_last_of(' ') + 1, loc.find(';', loc.find_last_of(' ')) - loc.find_last_of(' ') - 1);
}

Redirect::Redirect(const Redirect& other): Location(other), _code(other._code), _redirect(other._redirect) {}

Redirect::~Redirect() {}

int Redirect::code() const { return _code; }

std::string Redirect::redirect() const { return _redirect; }

Index::Index(std::string& loc) {
	_path = loc.substr(loc.find("location") + 9, loc.find('{') - loc.find("location") - 10);
	_file = loc.substr(loc.find("root") + 5, loc.find(';', loc.find("root")) - loc.find("root") - 5);
	_index = loc.substr(loc.find("file") + 5, loc.find(';', loc.find("file")) - loc.find("file") - 5);
	_autoindex = loc.find("autoindex on") != std::string::npos;
	_cgi = loc.find("cgi on") != std::string::npos;

	std::string methods = loc.substr(loc.find("methods") + 8, loc.find(';', loc.find("methods")) - loc.find("methods") - 8);
	std::stringstream ss(methods);
	std::string method;
	while (std::getline(ss, method, ' ')) {
		_methods.push_back(method);
	}
}

Index::Index(const Index& other): Location(other), _file(other._file), _index(other._index), _autoindex(other._autoindex), _cgi(other._cgi),_methods(other._methods) {}

Index::~Index() {}

std::string Index::file() {	return _file; }

std::string Index::index() { return _index; }

bool Index::autoindex() { return _autoindex; }

bool Index::cgi() { return _cgi; }

std::list<std::string> Index::methods() { return _methods; }
