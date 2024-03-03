#include "Server.hpp"
#include "../utils/Utils.hpp"

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

Server::Server(std::string& serverBlock): _fd(-1) {
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

int Server::fd() const { return _fd; }

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
	return NULL;
}

Location* Server::bestLocation(const std::string& path) const {
	std::size_t longestMatch = 0;
	Location* bestLocation = NULL;
	for (std::list<Location*>::const_iterator it = _locations.begin(); it != _locations.end(); ++it) {
		Location* location = *it;
	}
	for (std::list<Location*>::const_iterator it = _locations.begin(); it != _locations.end(); ++it) {
		Location* location = *it;
		if (location->path().length() > longestMatch) {
			if (path.find(location->path()) == 0) {
				bestLocation = location;
				longestMatch = location->path().length();
			}
		}
	}
	return bestLocation;
}



void Server::bind() {

  struct sockaddr_in servaddr = {};
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  try {
    if (_port <= 0 || _port > 65535)
      throw std::runtime_error("Invalid port number\n");
    servaddr.sin_port = htons(_port);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }

  ((_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
      ? throw std::runtime_error("Socket creation failed\n")
      : Logger::debug("Socket successfully created");

  // set socket as reusable

	int option = 1;
	((setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) == -1))
		? throw std::runtime_error("Setting to reusable failed\n")
		: Logger::debug("Socket options successfully setted");

	// https://man7.org/linux/man-pages/man2/bind.2.html
	(::bind(_fd, reinterpret_cast<struct sockaddr *>(&servaddr), sizeof(servaddr)) == -1)
		? throw std::runtime_error("Binding failed\n")
		: Logger::debug("Socket successfully binded");

	// https://man7.org/linux/man-pages/man2/listen.2.html
	try {
		listen(_fd, 256);
		Logger::info("Server listening on port " + Utils::toString(_port));
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}

	// setting socket as non blocking
	(fcntl(_fd, F_SETFL, O_NONBLOCK) == -1)
		? throw std::runtime_error("Setting to nonblocking failed")
		: Logger::debug("Socket setted to non blocking");

	Logger::info("Server " + _server_name + " listening in port " + Utils::toString(_port));
	}
