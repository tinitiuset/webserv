#ifndef SERVER_HPP
#define SERVER_HPP
#include <string>
#include <list>

#include "Location.hpp"

class Server {

private:
	std::string _server_name;
	std::string _address;
	int _port;
	int _body_size;
	std::string _root;
	std::list <Location*> _locations;

public:
	Server(std::string&);
	Server(const Server& other);
	~Server();

	std::string server_name() const;
	std::string address() const;
	int port() const;
	int body_size() const;
	std::string root() const;
	Location* location(const std::string& path) const;

};

#endif
