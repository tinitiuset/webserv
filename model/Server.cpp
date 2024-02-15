#include "Server.hpp"

void sanitize(std::string& serverBlock) {
	std::string result;
	char prevChar = 0;
	for (size_t i = 0; i < serverBlock.size(); ++i) {
		if (serverBlock[i] == ' ' && prevChar == '\n') {
			continue;
		} else {
			result += serverBlock[i];
		}
		prevChar = serverBlock[i];
	}
	serverBlock = result;

	serverBlock.erase(std::remove(serverBlock.begin(), serverBlock.end(), '\n'), serverBlock.end());
}

Server::Server(std::string& serverBlock) {
	sanitize(serverBlock);

	size_t pos = 0;
	while ((pos = serverBlock.find("location", pos)) != std::string::npos) {
		size_t endPos = serverBlock.find('}', pos);
		if (endPos == std::string::npos) {
			break;
		}

		std::string locationBlock = serverBlock.substr(pos, endPos - pos + 1);

		Location* location;
		(locationBlock.find("redirect") != std::string::npos) ?
			location = new Redirect(locationBlock)
		:
			location = new Index(locationBlock);
		_locations.push_back(location);

		pos = endPos + 1;
	}

	pos = serverBlock.find("listen");
	if (pos != std::string::npos) {
		_address = serverBlock.substr(pos + 7, serverBlock.find(':', pos) - pos - 7);
		_port = std::stoi(serverBlock.substr(serverBlock.find(':', pos) + 1, serverBlock.find(';', pos) - serverBlock.find(':', pos) - 1));
	}

	pos = serverBlock.find("server_name");
	if (pos != std::string::npos) {
		_server_name = serverBlock.substr(pos + 12, serverBlock.find(';', pos) - pos - 12);
	}

	pos = serverBlock.find("body_size");
	if (pos != std::string::npos) {
		_body_size = std::stoi(serverBlock.substr(pos + 10, serverBlock.find(';', pos) - pos - 10));
	}

	pos = serverBlock.find("root");
	if (pos != std::string::npos) {
		_root = serverBlock.substr(pos + 5, serverBlock.find(';', pos) - pos - 5);
	}
}

Server::Server(const Server& other): _server_name(other._server_name), _address(other._address), _port(other._port),
  _body_size(other._body_size), _root(other._root)
{
	for (std::list<Location*>::const_iterator it = other._locations.begin(); it != other._locations.end(); ++it) {
		_locations.push_back((*it)->clone());
	}
}

Server::~Server() {
	for (std::list<Location*>::iterator it = _locations.begin(); it != _locations.end(); ++it)
		delete *it;
}

std::string Server::server_name() const { return _server_name; }

std::string Server::address() const { return _address; }

int Server::port() const { return _port; }

int Server::body_size() const {	return _body_size; }

std::string Server::root() const { return _root; }

Location* Server::location(const std::string& path) const {
	for (std::list<Location*>::const_iterator it = _locations.begin(); it != _locations.end(); ++it) {
		Location* location = *it;
		if (path.find(location->path()) == 0)
			return location;
	}
	// Should be default location TBD
	return nullptr;
}
