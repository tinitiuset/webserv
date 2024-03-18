#include "Conf.hpp"

Conf::Conf(const std::string& file): _file(file) {}

Conf::~Conf() {}

bool Conf::parse() {
	std::ifstream file(_file.c_str());
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << _file << std::endl;
		return false;
	}

	std::string line;
	std::string serverBlock;
	int braceCount = 0;
	while (std::getline(file, line)) {

		if (line.empty()) continue;

		if (line.find("server") != std::string::npos && braceCount == 0) {
			if (!serverBlock.empty()) {
				_servers.push_back(Server(serverBlock));
			}
			serverBlock.clear();
		}

		serverBlock += line + "\n";

		if (line.find('{') != std::string::npos) {
			braceCount++;
		}
		if (line.find('}') != std::string::npos) {
			braceCount--;
			if (braceCount == 0 && !serverBlock.empty()) {
				_servers.push_back(Server(serverBlock));
				serverBlock.clear();
			}
		}
	}

	file.close();
	return true;
}

void Conf::load() {
	for (size_t i = 0; i < _servers.size(); ++i)
		_servers[i].bind();
}

void Conf::validate() const {
	for (size_t i = 0; i < _servers.size(); ++i)
		_servers[i].validate();
}

const Server& Conf::getServer(int port) const
{
	size_t i;
	for (i = 0; i < _servers.size(); ++i)
		if (_servers[i].port() == port)
			return (_servers[i]);
	throw std::runtime_error("Server not found for the specified port");
}

int Conf::serverCount() {
	return _servers.size();
}

std::vector<int> Conf::serverSockets() {
	std::vector<int> sockets;
	for (size_t i = 0; i < _servers.size(); ++i)
		sockets.push_back(_servers[i].fd());
	return sockets;
}