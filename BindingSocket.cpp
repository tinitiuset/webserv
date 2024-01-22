#include "BindingSocket.hpp"

BindingSocket::BindingSocket(int domain, int service, int protocol, int port, u_long interface):
ASocket(domain, service, protocol, port, interface)
{
    // or int conn = connectToNetwork()...
    _connection = connectToNetwork(_sock, _address);
    testConnection(_connection);
}

BindingSocket::~BindingSocket() {}

int        BindingSocket::connectToNetwork(int sock, struct sockaddr_in address)
{
    return (bind(sock, reinterpret_cast<struct sockaddr*>(&address), sizeof(address)));
}