#include "Location.hpp"

Location::Location() {}

Location::~Location() {}

std::string Location::path() const { return _path; }

void Location::validate() const {
	_path.empty() ? throw std::runtime_error("Location path is empty") : 0;
}

Redirect::Redirect(std::string& loc): Location() {
	std::stringstream ss(loc);

	std::string line;
	std::getline(ss, line);
	_path = line.substr(line.find("location") + 9, line.find('{') - line.find("location") - 10);

	while (std::getline(ss, line)) {
		if (line.find("redirect") != std::string::npos) {
			_redirect = line.substr(line.find(' ', line.find("redirect")) + 1, line.find(';', line.find("redirect")) - line.find(' ', line.find("redirect")) - 1);
			_code = Utils::toInt(_redirect.substr(0,3));
			_redirect = _redirect.substr(_redirect.find_last_of(' ') + 1, _redirect.length());
		}
	}
}

Redirect::Redirect(const Redirect& other): Location(other), _code(other._code), _redirect(other._redirect) {}

Redirect::~Redirect() {}

void Redirect::validate() const {
	Location::validate();
	_code < 300 || _code > 308 ? throw std::runtime_error("Redirect code is invalid") : 0;
	_redirect.empty() ? throw std::runtime_error("Redirect redirect is empty") : 0;
}

int Redirect::code() const { return _code; }

std::string Redirect::redirect() const { return _redirect; }

Index::Index(std::string& loc): Location(), _autoindex(false), _cgi(false), _methods() {
	std::stringstream ss(loc);

	std::string line;
	std::getline(ss, line);
	_path = line.substr(line.find("location") + 9, line.find('{') - line.find("location") - 10);

	while (std::getline(ss, line)) {
		if (line.find("root") != std::string::npos)
			_root = line.substr(line.find("root") + 5, line.find(';', line.find("root")) - line.find("root") - 5);
		if (line.find("file") != std::string::npos)
			_file = line.substr(line.find("file") + 5, line.find(';', line.find("file")) - line.find("file") - 5);
		if (line.find("autoindex on;") != std::string::npos)
			_autoindex = true;
		if (line.find("autoindex off;") != std::string::npos)
			_autoindex = false;
		if (line.find("cgi on") != std::string::npos)
			_cgi = true;
		if (line.find("cgi off") != std::string::npos)
			_cgi = false;
		if (line.find("methods") != std::string::npos) {
			std::string methods = line.substr(line.find("methods") + 8, line.find(';', line.find("methods")) - line.find("methods") - 8);
			std::stringstream ss(methods);
			std::string method;

			while (std::getline(ss, method, ' '))
				_methods.push_back(method);
		}
	}
}

Index::Index(const Index& other): Location(other), _root(other._root), _file(other._file), _autoindex(other._autoindex), _cgi(other._cgi),_methods(other._methods) {}

Index::~Index() {}

void Index::validate() const {
	Location::validate();
	_root.empty() ? throw std::runtime_error("Index root is empty") : 0;
}

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
