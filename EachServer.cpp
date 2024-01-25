#include "EachServer.hpp"

void    EachServer::setServerName(std::string servname)
{
    _serverName = servname;
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

void    EachServer::setBodySize(int size)
{
    _bodySize = size;
}

void    EachServer::setLocationOutKey(std::string str)
{
    _locations[str];
}

void    EachServer::setLocationInValue(std::string outKey, std::string inKey, std::string inVal)
{
    _locations[outKey][inKey] = inVal;
}

std::string         EachServer::getServerName()
{
    return (_serverName);
}

std::string         EachServer::getIp()
{
    return (_ip);
}

std::string         EachServer::getRoot()
{
    return (_root);
}

std::string         EachServer::getErrorPath(int code)
{
    return (_errors[code]);
}

std::vector<int>    EachServer::getPorts()
{
    return (_ports);
}

int                 EachServer::getBodySize()
{
    return (_bodySize);
}

std::string         EachServer::getLocationValue(std::string outKey, std::string inKey)
{
    return (_locations[outKey][inKey]);
}
