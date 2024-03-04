#include "Location.hpp"

Location::Location() {}

Location::~Location() {}

std::string Location::path() const { return _path; }

Redirect::Redirect(std::string& loc): Location() {
	_path = loc.substr(loc.find("location") + 9, loc.find('{') - loc.find("location") - 10);
	_code = std::stoi(loc.substr(loc.find("{redirect") + 10, loc.find(';', loc.find("{redirect")) - loc.find("{redirect") - 10));
	_redirect = loc.substr(loc.find_last_of(' ') + 1, loc.find(';', loc.find_last_of(' ')) - loc.find_last_of(' ') - 1);
}

Redirect::Redirect(const Redirect& other): Location(other), _code(other._code), _redirect(other._redirect) {}

Redirect::~Redirect() {}

int Redirect::code() const { return _code; }

std::string Redirect::redirect() const { return _redirect; }

Index::Index(std::string& loc): Location(), _autoindex(false), _cgi(false), _methods() {
	if (loc.find("location") != std::string::npos)
		_path = loc.substr(loc.find("location") + 9, loc.find('{') - loc.find("location") - 10);
	if (loc.find("root") != std::string::npos)
		_root = loc.substr(loc.find("root") + 5, loc.find(';', loc.find("root")) - loc.find("root") - 5);
	if (loc.find("file") != std::string::npos)
		_file = loc.substr(loc.find("file") + 5, loc.find(';', loc.find("file")) - loc.find("file") - 5);
	if (loc.find("autoindex on") != std::string::npos)
		_autoindex = true;
	if (loc.find("cgi on") != std::string::npos)
		_cgi = true;

	if (loc.find("methods") != std::string::npos) {
		std::string methods = loc.substr(loc.find("methods") + 8, loc.find(';', loc.find("methods")) - loc.find("methods") - 8);
		std::stringstream ss(methods);
		std::string method;

		while (std::getline(ss, method, ' '))
			_methods.push_back(method);
	}
}

Index::Index(const Index& other): Location(other), _root(other._root), _file(other._file), _autoindex(other._autoindex), _cgi(other._cgi),_methods(other._methods) {}

Index::~Index() {}

std::string Index::root() {	return _root; }

std::string Index::file() { return _file; }

bool Index::autoindex() { return _autoindex; }

bool Index::cgi() { return _cgi; }

std::list<std::string> Index::methods() { return _methods; }

std::string Index::buildRealPath(std::string &uri)
{
	std::string locT = Utils::removeLastSlash(path());
	std::string rootT = Utils::removeLastSlash(root());
	std::string uriT = Utils::removeLastSlash(uri);

	if (uriT == locT)
		return (rootT + "/" + file());
	rootT += "/";
	return (Utils::strReplace(uriT, locT, rootT));
}

bool Index::isMethodAllowed(std::string method)
{
	std::list<std::string>::iterator it = _methods.begin();
	std::list<std::string>::iterator ite = _methods.end();

	while (it != ite)
	{
		if (*it == method)
			return (true);
		it++;
	}
	return (false);
}
