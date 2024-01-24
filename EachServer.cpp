#include "EachServer.hpp"

void    EachServer::setServerName(std::string servname)
{
    _server_name = servname;
}

void    EachServer::setIp(std::string ip)
{
    _ip = ip;
}

void    EachServer::setRoot(std::string root)
{
    _root = root;
}

void    EachServer::setErrors(int num, std::string msg)
{
    _errors[num] = msg;
}

void    EachServer::setPorts(int port)
{
    _ports.push_back(port);
}
