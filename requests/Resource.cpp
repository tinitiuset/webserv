#include "Resource.hpp"

Resource::Resource(std::string path) {
	Logger::debug("Resource::Resource() creating resource with path " + path);
	_status = "HTTP/1.1 200 OK";
	_path = path;
}

Resource::Resource(const Resource& resource) {
	_path = resource._path;
}

Resource& Resource::operator=(const Resource& resource) {
	_path = resource._path;
	return *this;
}

Resource::~Resource() {}

std::string Resource::load() const {
	Logger::info("Resource::load() Loading resource from " + _path);
	std::ifstream file(_path);
	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	return content;
}

std::string Resource::status() const {
	return _status;
}
