#include "Conf.hpp"

Conf::Conf(const std::string& file): _file(file) {}

Conf::~Conf() {}

void Conf::parse() {
	std::ifstream file(_file);
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << _file << std::endl;
		return;
	}

	std::string line;
	std::string serverBlock;
	int braceCount = 0;
	while (std::getline(file, line)) {
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
}

void Conf::load() {
	for (size_t i = 0; i < _servers.size(); ++i)
		_servers[i].bind();
}

Server Conf::getServer(int index) {
	return _servers[index];
}

int Conf::getServerCount() {
	return _servers.size();
}

std::vector<int> Conf::getServerSockets() {
	std::vector<int> sockets;
	for (size_t i = 0; i < _servers.size(); ++i)
		sockets.push_back(_servers[i].fd());
	return sockets;
}